<?php

	function LoadNamedArrayFromTextFile($strFile)
	{
		$arrayContents = array();

		$ea = new CErrorAmnesty();
		$ea->Start();
		$file = fopen($strFile, "r");
		$ea->End();

		if ($file == 0)
		{
			return false;
		}

		while (!feof($file))
		{
			$strToken = fgets($file,1000);
			$strToken = chop($strToken);
			$strToken = u($strToken);
			$strValue = "";
			if (!feof($file))
			{
				$strValue = fgets($file,1000);
				$strValue = chop($strValue);
				$strValue = u($strValue);
			}
			if ($strValue != "")
			{
				$arrayContents[$strToken] = $strValue;
			}
		}
		
		fclose($file);
		return $arrayContents;
	}
	
	function LoadNamedArrayFromTextFileMultiLine($strFile, $strPrefix)
	{
		mb_internal_encoding("UTF-8");

		$arrayContentsRaw = array();
		$arrayContents = array();
		$ea = new CErrorAmnesty();
		$ea->Start();
		$arrayContentsRaw = file($strFile);
		$ea->End();
		if ($arrayContentsRaw == false)
		{
			return false;	
		}
		
		$nArrayLength = ArrayCount($arrayContentsRaw);
		if ($nArrayLength == 0)
		{
			return false;
		}
		
		$nPrefixLength = StringLength($strPrefix);
		$strNextID = u("");
		$strNextContents = u("");
		for ($nIndex = 0; $nIndex < $nArrayLength; $nIndex++)
		{
			$strCurrentLine = StringEncodeUTF8($arrayContentsRaw[$nIndex]);
			//print($strCurrentLine . "<br>");
			if (StringSection($strCurrentLine,0,$nPrefixLength) == $strPrefix)
			{
				if (($strNextID != u("")) && ($strNextContents != u("")))
				{
					$arrayContents[chop($strNextID)] = chop($strNextContents);
					$strNextID = u("");
					$strNextContents = u("");
				}
				$strNextID = $strCurrentLine;
			}
			else
			{
				$strNextContents .= $strCurrentLine;
			}
		}

		if (($strNextID != u("")) && ($strNextContents != u("")))
		{
			$arrayContents[chop($strNextID)] = chop($strNextContents);
		}
		return $arrayContents;
	}

	

	function SaveNamedArrayToTextFile($strFile, $arrayContents)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$file = fopen($strFile, "w");
		$ea->End();

		if ($file == 0)
			return false;

		for (reset($arrayContents); $strKey = key($arrayContents); next($arrayContents))
		{
			fputs($file,$strKey."\r\n");
			fputs($file,pos($arrayContents)."\r\n");
		}
	
		fclose($file);

		return true;
	}


