<?php

	function FileReadBinary($strFile)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$strContents = file_get_contents($strFile);
		$ea->End();
		return $strContents;	
 	}   	

	function FileRead($strFile)
	{
		$strContents = FileReadBinary($strFile);
		if (!is_string($strContents))
		{
			return false;	
		}   	
		$strContents = ReplaceString($strContents,"\r\n","\n");
		return StringEncodeUTF8($strContents);
	}
   
	function FileReadToArray($strFile)
	{
		$arrayContents = false;
   	$ea = new CErrorAmnesty();
   	$ea->Start();
		$arrayContents = file($strFile);
		$ea->End();		
		if ($arrayContents === false)
		{
			return false;	
		}
		foreach ($arrayContents as &$strLine)
		{
			$strLine = StringEncodeUTF8(chop($strLine));
		}
		return $arrayContents;
	}
	
	function FileAddText($strFile, $strText)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$file = fopen($strFile,'a');
		$ea->End();
		if ($file != 0)
		{
			fwrite($file, $strText);
			fclose($file);
			return true;
		}
		return false;
	}
        
        
	function FileWrite($strFile, $strData, $bOverwrite = false)
	{
		$strMode = ($bOverwrite == true)?'w+':'x+';
		$ea = new CErrorAmnesty();
		$ea->Start();
		$file = fopen($strFile,$strMode);
		$ea->End();
		if ($file != 0)
		{
			fwrite($file, $strData);
			fclose($file);
			return true;
		}
		return false;			
	}

	 

	

	
	



