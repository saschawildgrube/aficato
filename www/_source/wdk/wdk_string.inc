<?php

	function StringLength($strString)
	{
		if (is_string($strString) == false)
		{
			return 0;	
		}
		return mb_strlen($strString,"UTF-8");
	}
	
	function StringCount($strHaystack,$strNeedle,$bIgnoreCase=false)
	{
		if ($strNeedle == "")
		{
			return 0;	
		}
		if ($bIgnoreCase == true)
		{
			$strHaystack = StringLowerCase($strHaystack);
			$strNeedle = StringLowerCase($strNeedle);
		}
		return mb_substr_count($strHaystack,$strNeedle,"UTF-8");
	}
	function StringCountIgnoreCase($strHaystack,$strNeedle)
	{
		return StringCount($strHaystack,$strNeedle,true);
	}	

	function FindString($strHaystack, $strNeedle, $bIgnoreCase = false, $nOffset = 0)
	{
		$nHaystackLength = StringLength($strHaystack);
		if ($nHaystackLength == 0)
		{
			return -1;	
		}
		if (StringLength($strNeedle) == 0)
		{
			return -1;
		}
		if ($nOffset > 0)
		{
			if ($nHaystackLength < $nOffset)
			{
				return -1;	
			}	
		}
		if ($bIgnoreCase == true)
		{
			$nPos = mb_stripos($strHaystack, $strNeedle, $nOffset, "UTF-8");
		}
		else
		{
			$nPos = mb_strpos($strHaystack, $strNeedle, $nOffset, "UTF-8");
		}
		if ($nPos === false)
		{
			return -1;	
		}
		return $nPos;
	}

/*
	function FindString($strHaystack, $strNeedle, $bIgnoreCase = false, $nOffset = 0)
	{
		if (StringLength($strHaystack) == 0)
		{
			return -1;	
		}
		if (StringLength($strNeedle) == 0)
		{
			return -1;
		}
		if ($bIgnoreCase == true)
		{
			// mb_stripos is not supported until PHP 5.2.0
			$strHaystack = StringLowerCase($strHaystack);	
			$strNeedle = StringLowerCase($strNeedle);	
		}
		if ($nOffset > 0)
		{
			if (StringLength($strHaystack) < $nOffset)
			{
				return -1;	
			}	
		}
		$nPos = mb_strpos($strHaystack, $strNeedle, $nOffset, "UTF-8");
		if ($nPos === false)
		{
			return -1;	
		}
		return $nPos;
	}
*/

	function FindStringIgnoreCase($strHaystack, $strNeedle)
	{
		return FindString($strHaystack, $strNeedle, true);
	}
	
	function FindStringMultiple($strHaystack, $arrayNeedles, &$strNextNeedle, $bIgnoreCase = false, $nOffset = 0)
	{
		$strNextNeedle = "";
		if (!is_array($arrayNeedles))
		{
			return -1;
		}
		if ($nOffset > 0)
		{
			if (StringLength($strHaystack) < $nOffset)
			{
				return -1;	
			}	
		}
		$arrayResult = array();
		$nResult = -1;
		foreach ($arrayNeedles as $strNeedle)
		{
			$nNextPos = FindString($strHaystack,$strNeedle,$bIgnoreCase,$nOffset);
			if ($nNextPos != -1)
			{
				if ($nResult == -1 || $nNextPos < $nResult)
				{
					$nResult = $nNextPos;	
					$strNextNeedle = $strNeedle;
				}
			}
			if ($nResult == $nOffset)
			{
				break;	
			}
		}
		return $nResult;
	}
	function FindStringMultipleIgnoreCase($strHaystack, $arrayNeedles, &$strNextNeedle, $nOffset = 0)
	{
		return FindStringMultiple($strHaystack, $arrayNeedles, $strNextNeedle, true, $nOffset);
	}

/*
	function ReplaceString($strString,$strFrom,$strTo,$bIgnoreCase = false)
	{
		
		//$strString = StringEncodeUTF8($strString);
		//$strFrom = StringEncodeUTF8($strFrom);
		$nStrFromLength = StringLength($strFrom);
		//$strTo = StringEncodeUTF8($strTo);

    	$strRemainingString = $strString;
    	$strString = "";
		while(($nPos = FindString($strRemainingString, $strFrom, $bIgnoreCase)) != -1) 
    	{
    		$strString .= mb_substr($strRemainingString, 0, $nPos,"UTF-8");
    		$strString .= $strTo;
    		$strRemainingString = mb_substr($strRemainingString, $nPos+$nStrFromLength,StringLength($strRemainingString),"UTF-8"); 
		}
		return $strString . $strRemainingString;
	}
*/

	function ReplaceString($strString,$strFrom,$strTo,$bIgnoreCase = false)
	{
		if ($bIgnoreCase == true)
		{
			if (IsStringUTF8($strString) || IsStringUTF8($strFrom) || IsStringUTF8($strTo))
			{
				$nStrFromLength = StringLength($strFrom);
				$strRemainingString = $strString;
				$strString = "";
				while(($nPos = FindString($strRemainingString, $strFrom, $bIgnoreCase)) != -1) 
				{
					$strString .= mb_substr($strRemainingString, 0, $nPos,"UTF-8");
					$strString .= $strTo;
					$strRemainingString = mb_substr($strRemainingString, $nPos+$nStrFromLength,StringLength($strRemainingString),"UTF-8"); 
				}
				return $strString . $strRemainingString;
			}
			else
			{
				return str_ireplace($strFrom,$strTo,$strString);
			}
		}
		else
		{
			return str_replace($strFrom,$strTo,$strString);	
		}
	}
		


	function ReplaceStringIgnoreCase($strString,$strFrom,$strTo)
	{
		return ReplaceString($strString,$strFrom,$strTo,true);
	}


	function ReplaceStringOnce($strString,$strFrom,$strTo,$bIgnoreCase = false)
	{
		$nStrFromLength = StringLength($strFrom);
    	$strRemainingString = $strString;
    	$strString = "";
		$nPos = FindString($strRemainingString, $strFrom, $bIgnoreCase);
		if ($nPos != -1)
    	{
    		$strString .= mb_substr($strRemainingString, 0, $nPos,"UTF-8");
    		$strString .= $strTo;
    		$strRemainingString = mb_substr($strRemainingString, $nPos+$nStrFromLength,StringLength($strRemainingString),"UTF-8");
		}
		return $strString . $strRemainingString;
	}

	function ReplaceStringOnceIgnoreCase($strString,$strFrom,$strTo)
	{
		return ReplaceStringOnce($strString,$strFrom,$strTo,true);
	}
	
	function StringLowerCase($strString)
	{
		return mb_strtolower($strString,"UTF-8");	
	}
	function StringUpperCase($strString)
	{
		return mb_strtoupper($strString,"UTF-8");	
	}
	
	function StringRemoveControlChars($strString)
	{
		$strString = ReplaceString($strString,"\n","");
		$strString = ReplaceString($strString,"\r","");
		$strString = ReplaceString($strString,"\t","");
		$strString = trim($strString);
		return $strString;
	}

	function StringSlice($strCake, $nSlices)
	{
		$strCake = StringEncodeUTF8($strCake);
		$arraySlices = array();
		for ($nIndex=0; $nIndex < $nSlices; $nIndex++)
		{
			$arraySlices[$nIndex] = '';	
		}
		$nLength = StringLength($strCake);
		for ($nIndex=0; $nIndex < $nLength; $nIndex++)
		{
			$nCurrentSlice = $nIndex % $nSlices;
			$arraySlices[$nCurrentSlice].= mb_substr($strCake,$nIndex,1,'UTF-8');
		}
		return $arraySlices;
	}
	
	function StringGlue($arraySlices)
	{
		if (!is_array($arraySlices))
		{
			return "";	
		}
		if (ArrayCount($arraySlices) < 1)
		{
			return "";
		}
		$strFirstSlice = StringEncodeUTF8($arraySlices[0]);
		$nLength = StringLength($strFirstSlice);
		$strGlued = "";
		for ($nIndex = 0; $nIndex < $nLength; $nIndex++)
		{
			foreach ($arraySlices as $strSlice)
			{
				$strSlice = StringEncodeUTF8($strSlice);
				$strCharAtIndex = StringSection($strSlice,$nIndex,1);
				if ($strCharAtIndex != "")
				{
					$strGlued .= $strCharAtIndex;
				}
			}
		}
		return $strGlued;
	}


	function StringAddEmbracingTags(
		$strString,
		$strToken,
		$strStartTag,
		$strEndTag,
		$bIgnoreCase = false)
	{
		if ($strToken == "")
		{
			return $strString;
		}
		$strResult = "";
		$strRemaining = $strString;
		while (($nNextPos = FindString($strRemaining,$strToken,$bIgnoreCase)) != -1)
		{
			/*
			V = nNextPos
			remaining text with the token inside
			remaining text with the <tagged>token</tagged> inside.
			*/
			$strResult .= mb_substr($strRemaining,0,$nNextPos);
			$strResult .= $strStartTag;
			$strResult .= mb_substr($strRemaining,$nNextPos,StringLength($strToken));
			$strResult .= $strEndTag;
			$strRemaining = mb_substr($strRemaining,$nNextPos + StringLength($strToken));
		}
		return $strResult . $strRemaining;
	}


	function StringAddEmbracingTagsIgnoreCase(
		$strString,
		$strToken,
		$strStartTag,
		$strEndTag)
	{
		return StringAddEmbracingTags(
			$strString,
			$strToken,
			$strStartTag,
			$strEndTag,
			true);
	}
	
	function StringGetFromEmbracingTags(
		$strString,
		$strStartTag,
		$strEndTag)
	{
		$strResult = $strString;
		$nPosStart = FindStringIgnoreCase($strResult,$strStartTag);
		if ($nPosStart == -1)
		{
			return false;	
		}
		$strResult = StringSection($strResult,$nPosStart+StringLength($strStartTag));
		$nPosEnd = FindStringIgnoreCase($strResult,$strEndTag);
		if ($nPosEnd == -1)
		{
			return false;	
		}
		$strResult = StringSection($strResult,0,$nPosEnd);
		return $strResult;
	}
	
	function StringGetEndOfLineToken($strString)
	{
		if ($strString == "")	
		{
			return "";	
		}
		if (FindString($strString,"\r\n") != -1)
		{
			return "\r\n";	
		}
		if (FindString($strString,"\n\r") != -1)
		{
			return "\n\r";	
		}
		if (FindString($strString,"\n") != -1)
		{
			return "\n";	
		}
		return "";
	}
	
	function StringFilter($strInput,$strFilter)
	{
		if (StringLength($strFilter) == 0)
		{
			return '';
		}
		$strOutput = '';
		for ($nIndex = 0; $nIndex < StringLength($strInput); $nIndex++)
		{
			if (FindString($strFilter,$strInput[$nIndex]) != -1)
			{
				$strOutput .= $strInput[$nIndex];
			}
		}
		return $strOutput;
	}	
	

	function ReplaceTags($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix)
	{
		// ReplaceTags_SearchNextTag is defect. It can't cope with pseudo prefixes.
		$nTagCount = ArrayCount($arrayTags);
		if ($nTagCount < 50)
		{
			return ReplaceTags_TagLoop($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);
		}
		/*$nPrefixCount = StringCount($strTemplate, $strTagPrefix);
		if ($nPrefixCount < 500)
		{
			return ReplaceTags_SearchNextTag($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);
		}
		*/
		return ReplaceTags_SearchNextTag($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);
		//return ReplaceTags_TagLoop($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);
	}
	
	function ReplaceTags_TagLoop($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix)
	{
		if ($strTagPrefix != '')
		{
			if (FindString($strTemplate,$strTagPrefix) == -1 )
			{
				return $strTemplate;
			}
		}
		$strResult = $strTemplate;
		$nCounter = 0;
		foreach ($arrayTags as $strKey => $strValue)
		{
			$strResult = ReplaceString(
		   	$strResult,
		   	$strTagPrefix.$strKey.$strTagPostfix,
		   	$strValue);
		   	
		  $nCounter++;
		  if ($nCounter == 10)
		  {
		  	if ($strTagPrefix != '')
		  	{
					if (FindString($strResult,$strTagPrefix) == -1 )
					{
						return $strResult;
					}
					$nCounter = 0;
				}
			}
		}
		return $strResult;
	}

	
	function ReplaceTags_SearchNextTag($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix)
	{
		if (!is_array($arrayTags))
		{
			return $strTemplate;	
		}
		$nTagPrefixLength = StringLength($strTagPrefix);
		$nTagPostfixLength = StringLength($strTagPostfix);

		if ($nTagPrefixLength == 0 || $nTagPostfixLength == 0)
		{
			return ReplaceTags_TagLoop($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);	
		}
	
		$strResult = '';
		$nPosNextTagPrefix = -1;
		$nPosNextTagPostfix = -1;
		$nPos = 0;
		
		while (1)
		{
			$nPosNextTagPrefix = strpos(
				$strTemplate,
				$strTagPrefix,
				$nPos);
			if ($nPosNextTagPrefix === false)	
			{
				return $strTemplate;
			}
			else
			{
				$nPos = $nPosNextTagPrefix+$nTagPrefixLength+1;
				// plus one because a tag name must be at least one character long.
				
				if ($nPos > strlen($strTemplate)-1) 
				{
					return $strTemplate;	
				}
				
				$nPosNextTagPostfix = strpos(
					$strTemplate,
					$strTagPostfix,
					$nPos);
				if ($nPosNextTagPostfix === false)	
				{
					return $strTemplate;
				}
				else
				{
					$strTag = substr(
						$strTemplate,
						$nPosNextTagPrefix+$nTagPrefixLength,
						$nPosNextTagPostfix-$nPosNextTagPrefix-$nTagPrefixLength);
						
					if (isset($arrayTags[$strTag]) == true)
					{
						
						//substr($strString,intval($nStart),intval($nLength))
						
						$strTemplate =
							substr($strTemplate,0,$nPosNextTagPrefix)
							.$arrayTags[$strTag]
							.substr($strTemplate,$nPosNextTagPostfix+$nTagPostfixLength);
						/*
						$strTemplate = str_replace(
						   	$strTagPrefix.$strTag.$strTagPostfix,
						   	$arrayTags[$strTag],
						   	$strTemplate);
						   	
						*/
					}
					$nPos = $nPosNextTagPrefix+$nTagPrefixLength;
				}
			}
		}
		return $strTemplate;
	}

	
	
	
	function ReplaceTags_SearchNextTag_Old($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix)
	{
		if (!is_array($arrayTags))
		{
			return $strTemplate;	
		}
		if ($strTagPrefix=='' || $strTagPostfix=='')
		{
			return ReplaceTags_TagLoop($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix);	
		}
		if (FindString($strTemplate,$strTagPrefix) == -1 )
		{
			return $strTemplate;
		}
	
		$nTagPrefixLength = StringLength($strTagPrefix);
		$strResult = '';
		$strRemaining = $strTemplate;
		
		$nDebug = 0;
		
		$bEnd = false;
		while (!$bEnd)
		{
			$nDebug++;
			$nPosNextTagPrefix = FindString($strRemaining,$strTagPrefix);
			if ($nPosNextTagPrefix == -1)	
			{
				$bEnd = true;
			}
			else
			{
				
				$strResult .= StringSection($strRemaining,0,$nPosNextTagPrefix);
				$strRemaining = StringSection($strRemaining,$nPosNextTagPrefix);
				$strRemainingAfterNextTagPrefix = StringSection($strRemaining,$nTagPrefixLength);
				
				$nPosNextTagPostfix = FindString(
					$strRemainingAfterNextTagPrefix,
					$strTagPostfix);
				if ($nPosNextTagPostfix == -1)	
				{
					$bEnd = true;
				}
				else
				{
					$nPosNextTagPostfix += $nTagPrefixLength;
					$strTag = StringSection($strRemaining,$nTagPrefixLength,$nPosNextTagPostfix-$nTagPrefixLength);
					
					if (isset($arrayTags[$strTag]) == false)
					{
						$strResult .= StringSection($strRemaining,0,$nPosNextTagPostfix);
						$strRemaining = StringSection($strRemaining,$nPosNextTagPostfix);
					}
					else
					{
						if (FindString($strRemaining,$strTagPrefix.$strTag.$strTagPostfix) == -1)
						{
							// it looked like a tag, but it wasn't	
							$strResult .= StringSection($strRemaining,$nTagPrefixLength);
							$strRemaining = $strRemainingAfterTagPrefix;
						}
						else
						{
							$strValue = $arrayTags[$strTag];
						   $strRemaining = ReplaceString(
						   	$strRemaining,
						   	$strTagPrefix.$strTag.$strTagPostfix,
						   	$strValue);
						}
					}
				}
			}
		}
		$strResult .= $strRemaining;
		return $strResult;
	}	
	
	
	
	
	
	function ReplaceTagsXML($strTemplate, $arrayTags)
	{
		return ReplaceTags($strTemplate, $arrayTags, '<', '/>');
	}
	
	function ReplaceTagsBraces($strTemplate, $arrayTags)
	{
	  	return ReplaceTags($strTemplate, $arrayTags, '{', '}');
	}	 
	
	function ReplaceTagsWDK($strTemplate, $arrayTags)
	{
	  	return ReplaceTagsBraces($strTemplate, $arrayTags);
	}	 	 



	function ReplaceTagsWithReplaceString($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix, $strSearch, $strReplace)
	{
		if (!is_array($arrayTags))
		{
			return $strTemplate;	
		}
		if (FindString($strTemplate,$strTagPrefix) == -1 )
		{
			return $strTemplate;
		}
		$strResult = $strTemplate;
		for (reset($arrayTags); $strKey = key($arrayTags); next($arrayTags))
		{
		   $strValue = $arrayTags[$strKey];
		   $strResult = str_replace(
		   	$strTagPrefix.$strKey.$strTagPostfix,
		   	str_replace($strSearch, $strReplace, $strValue),
		   	$strResult);
		}
		return $strResult;
	}


	
	/*
	function ReplaceTagsWithReplaceString($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix, $strSearch, $strReplace)
	{
		if (!is_array($arrayTags))
		{
			return $strTemplate;	
		}
		if (FindString($strTemplate,$strTagPrefix) == -1 )
		{
			return $strTemplate;
		}
		$strResult = $strTemplate;
		foreach ($arrayTags as $strKey => $strValue)
		{
		   $strResult = ReplaceString(
		   	$strResult,
		   	$strTagPrefix.$strKey.$strTagPostfix,
		   	ReplaceString(
		   		$strValue,
		   		$strSearch,
		   		$strReplace));
		}
		return $strResult;
	}
*/

/*
	function ReplaceTagsWithReplaceString($strTemplate, $arrayTags, $strTagPrefix, $strTagPostfix, $strSearch, $strReplace)
	{
		if (!is_array($arrayTags))
		{
			return $strTemplate;	
		}
		if (FindString($strTemplate,$strTagPrefix) == -1 )
		{
			return $strTemplate;
		}
	
		if (ArrayCount($arrayTags) < 100)
		{
			$strResult = $strTemplate; 
			foreach ($arrayTags as $strKey => $strValue)
			{
			   $strResult = ReplaceString(
			   	$strResult,
			   	$strTagPrefix.$strKey.$strTagPostfix,
			   	ReplaceString(
			   		$strValue,
			   		$strSearch,
			   		$strReplace));
			}
			return $strResult;
		}

		
		
		$nTagPrefixLength = StringLength($strTagPrefix);
		$strResult = "";
		$strRemaining = $strTemplate;
		
		$bEnd = false;
		while (!$bEnd)
		{
			$nPosNextTagPrefix = FindString($strRemaining,$strTagPrefix);
			if ($nPosNextTagPrefix == -1)	
			{
				$bEnd = true;
			}
			else
			{
				
				$strResult .= mb_substr($strRemaining,0,$nPosNextTagPrefix);
				$strRemaining = mb_substr($strRemaining,$nPosNextTagPrefix);
				$strRemainingAfterNextTagPrefix = mb_substr($strRemaining,$nTagPrefixLength);
				
				$nPosNextTagPostfix = FindString(
					$strRemainingAfterNextTagPrefix,
					$strTagPostfix);
				if ($nPosNextTagPostfix == -1)	
				{
					$bEnd = true;
				}
				else
				{
					$nPosNextTagPostfix += $nTagPrefixLength;
					$strTag = mb_substr($strRemaining,$nTagPrefixLength,$nPosNextTagPostfix-$nTagPrefixLength);
					
					
					//if (ArrayKeyExists($arrayTags,$strTag) == false)
					if (isset($arrayTags[$strTag]) == false)
					{
						$strResult .= mb_substr($strRemaining,0,$nPosNextTagPostfix);
						$strRemaining = mb_substr($strRemaining,$nPosNextTagPostfix);
					}
					else
					{
						if (FindString($strRemaining,$strTagPrefix.$strTag.$strTagPostfix) == -1)
						{
							// it looked like a tag, but it wasn't	
							$strResult .= mb_substr($strRemaining,$nTagPrefixLength);
							$strRemaining = $strRemainingAfterTagPrefix;
						}
						else
						{
							$strValue = $arrayTags[$strTag];
						   $strRemaining = ReplaceStringOnce(
						   	$strRemaining,
						   	$strTagPrefix.$strTag.$strTagPostfix,
						   	ReplaceString(
						   		$strValue,
						   		$strSearch,
						   		$strReplace));
						}
					}
				}
			}
		}
		$strResult .= $strRemaining;
		return $strResult;
	}

*/	
	

	function StringSection($strString,$nStart = 0, $nLength = false)
	{
		if ($nLength === false)
		{
			$nLength = StringLength($strString);
		}
		return mb_substr($strString,intval($nStart),intval($nLength),"UTF-8");	
	}

	
	function StringCutOff($strString,$nMaxLength = 100,$strTrailer = "...")
	{
		$strString = StringWordWrap($strString, $nMaxLength, "XXXXX",true); 
		$nPos = FindString($strString,"XXXXX");
		if ($nPos != -1)
		{
			$strString = mb_substr($strString,0,$nPos) . $strTrailer;
		}		
		return $strString;	
	}

	function StringWordWrap($strString, $nWidth = 75, $strEOL = "\n", $bCut = false)
	{
		mb_internal_encoding("UTF-8");
		$arrayLines = explode($strEOL,$strString);
		if ($arrayLines == false)
		{
			return $strString;
		}
		if ($nWidth < 1)
		{
			return $strString;	
		}
		$strResult = "";
		$nLineCount = ArrayCount($arrayLines);
		foreach ($arrayLines as $strLine)
		{
			$strRemainingLine = trim($strLine," ");
			while (StringLength($strRemainingLine) > 0)
			{
				$strRemainingLine = trim($strRemainingLine, " ");
				$nRemainingLength = StringLength($strRemainingLine);
				if ($nRemainingLength <= $nWidth)
				{
					// The line fits into the intended width
					$strResult .= $strRemainingLine;
					$strRemainingLine = "";
				}
				else
				{
					// The line must be wrapped
					$nNextWhiteSpacePos = FindString($strRemainingLine," ");
					if ($nNextWhiteSpacePos == -1)
					{
						$nNextWhiteSpacePos = $nRemainingLength;
					}
					if ($nNextWhiteSpacePos >= $nWidth)
					{
						// The next word is longer than the intended width.
						if ($bCut == true)
						{
							$strResult .= mb_substr($strRemainingLine,0,$nWidth);
							$strRemainingLine = mb_substr($strRemainingLine,$nWidth);
						}
						else 
						{
							$strResult .= mb_substr($strRemainingLine,0,$nNextWhiteSpacePos);
							$strRemainingLine = mb_substr($strRemainingLine,$nNextWhiteSpacePos);
						}
					}
					else
					{
						// the next word fits into the remaining line					

						$nLastWhiteSpacePos = mb_strrpos(mb_substr($strRemainingLine,0,$nWidth+1)," ");
						if ($nLastWhiteSpacePos == false)
						{
							$strResult .= $strRemainingLine;
							$strRemainingLine = "";
						}
						else
						{
							$strResult .= mb_substr($strRemainingLine,0,$nLastWhiteSpacePos);
							$strRemainingLine = mb_substr($strRemainingLine,$nLastWhiteSpacePos);
						}
					}
				}
				if ($strRemainingLine != "")
				{
					$strResult .= $strEOL;	
				}
			}
			$nLineCount--;
			if ($nLineCount > 0)
			{
				$strResult .= $strEOL;
			}
		}
		return $strResult;	
	}

	function StringRepeat($strString,$nCount)
	{
		return str_repeat($strString,$nCount);
	}


	function StringCheckCharSet($strString,$strCharsetDef)
	{
		$nLength = StringLength($strString);
		for ($nIndex = 0; $nIndex < $nLength; $nIndex++)
		{
			if (FindString($strCharsetDef,StringSection($strString,$nIndex,1)) == -1)
			{
				return false;
			}
		}
		return true;
	}

	
	
	function StringCheckCamelCase($strString)
	{
		if (ctype_upper(StringSection($strString,0,1)) == false)    
		{
			return false;
		}
		if (StringCheckCharSet($strString,CHARSET_ALPHANUMERIC) == false)
		{
			return false;
		}
		return true;	
	}
	
	function StringTranslateCharacters($strString,$strFrom,$strTo)
	{
		$nCharsetLength = min(StringLength($strFrom),StringLength($strTo));
		$strResult = $strString;
		for ($nIndex = 0; $nIndex < $nCharsetLength; $nIndex++)
		{
			$strResult = ReplaceString(
				$strResult,
				StringSection($strFrom,$nIndex,1),
				StringSection($strTo,$nIndex,1));
		}
		return $strResult;
	}
	
	function StringRemoveCharacters($strInput,$strBlacklist)     
	{
		mb_internal_encoding('UTF-8');
		$strInput = StringEncodeUTF8($strInput);
		$strOutput = '';
		for ($nIndex = 0; $nIndex < StringLength($strInput); $nIndex++)
		{
			if (FindString($strBlacklist,mb_substr($strInput,$nIndex,1)) == -1)
			{
				$strOutput .= mb_substr($strInput,$nIndex,1);
			}
		}
		return $strOutput;
	}
	
	function StringRemoveTrailingCharacters($strInput,$strBlacklist,$bMultiple = false)     
	{
		$strResult = $strInput;
		while ($strResult != "")
		{
			$strChar = StringSection($strResult,-1,1);  
			if (StringCheckCharSet($strChar,$strBlacklist) == false)
			{
				return $strResult;
			}
			else
			{
				$strResult = StringSection($strResult,0,-1);
				if ($bMultiple == false)
				{
					return $strResult;
				}
			}
		}
		return $strResult;
	}	
	
	function StringRemoveLeadingCharacters($strInput,$strBlacklist,$bMultiple = false)     
	{
		$strResult = $strInput;
		while ($strResult != "")
		{
			$strChar = StringSection($strResult,0,1);  
			if (StringCheckCharSet($strChar,$strBlacklist) == false)
			{
				return $strResult;
			}
			else
			{
				$strResult = StringSection($strResult,1);
				if ($bMultiple == false)
				{
					return $strResult;
				}
			}
		}
		return $strResult;
	}
	
	function StringTrim($strInput)
	{
		return trim("$strInput");	
	}
	
	function StringExplode($strInput,$strSeparator,$bTrim = false, $nLimit = 0)
	{
		if (is_array($strInput))
		{
			return array();	
		}
		if ($bTrim == true)
		{
			$strInput = StringTrim($strInput);	
		}
		if ($nLimit == 0)
		{
			$arrayValues = explode($strSeparator,$strInput);
		}
		else
		{
			$arrayValues = explode($strSeparator,$strInput,$nLimit);
		}
			
		if ($arrayValues == false)
		{
			return array();	
		}
		if ($bTrim == true)
		{
			foreach($arrayValues as &$strValue)
			{
				$strValue = StringTrim($strValue);	
			}	
		}
		return $arrayValues;  
	}
	
	
	function StringExplodeKeyValueList(
		$strKeyValueList,
		$strLineSeparator="\n",
		$strKeySeparator="=",
		$bTrim = true)
	{
		$arrayKeyValues = array();
		$arrayLines = StringExplode($strKeyValueList,$strLineSeparator,$bTrim);
		foreach ($arrayLines as $strLine)
		{
			$arrayItem = StringExplode($strLine,$strKeySeparator,$bTrim,2);
			$strKey = ArrayGetValue($arrayItem,0);
			if ($strKey != "")	
			{
				$strValue = ArrayGetValue($arrayItem,1);
				$arrayKeyValues[$strKey] = $strValue;
			}
		}	
		return $arrayKeyValues;
	}
	
	
	
