<?php

	require_once("wdk_typeconversion.inc");

	function RenderPathContent($arrayData,$strPathPrefix = "") 
	{
		if (!is_array($arrayData))
		{
			return "";	
		}
		$strData = "";
		foreach ($arrayData as $key => $value)   
		{
			$strKey = $strPathPrefix;
			if (is_numeric($key))
			{
				$strKey .= "#".$key;
			}
			else
			{
				$strKey .= StringURLEncode($key);
			}
			if (is_array($value))
			{
				if (ArrayCount($value) == 0)
				{
					$strData .= "$strKey,\n";
				}
				else
				{
					$strData .= RenderPathContent($value,$strKey."/")."\n";
				}
			}
			else
			{
				$strData .= $strKey . ",";
				$strData .= StringURLEncode($value)."\n"; 
			}
		}
		return StringTrim($strData);
	}
	
	function ParsePathContent($strData)
	{
		if ($strData == "")
		{
			return array();	
		}
		$strEOLToken = StringGetEndOfLineToken($strData);
		//print("1".RenderHex($strEOLToken)."2");
		if ($strEOLToken == "")
		{
			$arrayLines = array($strData);	
		}
		else
		{
			$arrayLines = explode($strEOLToken,$strData);
		}
		
		
		$arrayPathContent = array();
		foreach ($arrayLines as $strLine)
		{
			if ($strLine != "")
			{
				$arrayLine = explode(",",$strLine,2);
				if (is_array($arrayLine))
				{
					if (ArrayCount($arrayLine) == 2)
					{
						$strPath = trim($arrayLine[0]);
						if ($strPath != "")
						{
							$strContent = trim($arrayLine[1]);
							$arrayPathContent[$strPath] = $strContent;
						}
					}
				}
			}
		}
		
		$arrayData = array();
		
		foreach ($arrayPathContent as $strPath => $strContent)
		{
			$strContent = StringURLDecode($strContent);
			$arrayPathSegments = explode("/",$strPath);
			if (is_array($arrayPathSegments))
			{
				
				$strEvalCode = '$arrayData';
				if (ArrayCount($arrayPathSegments) > 0)
				{
					foreach ($arrayPathSegments as &$strPathSegment)
					{
						if (StringSection($strPathSegment,0,1) == "#")
						{
							$nIndex = intval(StringSection($strPathSegment,1));
							//$strPathSegment = intval(StringSection($strPathSegment,1));	
							$strEvalCode .= "[$nIndex]";
						}
						else
						{
							$strEvalCode .= "['" .ReplaceString(StringURLDecode($strPathSegment),"'","\\'"). "']";
						}
					}
				}
				else
				{
					$strEvalCode .= "[]";
				}
				
				$strEvalCode .= ' = $strContent;';
				eval($strEvalCode);
			}
			
		}
		
		return $arrayData;
	}


