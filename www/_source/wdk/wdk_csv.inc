<?php


	/*
	
		ParseCSV considers all specifications defined in RFC 4180
		plus the use of comment tags which mark the rest of the line
		as a comment (unless the comment tag is within a quoted value).
	
		Refer to:
		http://www.ietf.org/rfc/rfc4180.txt
	
	*/


	define("CSV_INITIAL",						0);
	define("CSV_LEADINGWHITESPACE",	1);
	define("CSV_UNQUOTEDVALUE",			2);
	define("CSV_QUOTESTART",				3);
	define("CSV_QUOTEDVALUE",				4);
	define("CSV_ESCAPEDQUOTE",			5);
	define("CSV_QUOTEEND",					6);
	define("CSV_TRAILINGNOISE",			7);
	define("CSV_SEPARATOR",					8);
	define("CSV_NEWLINE",						9);
	define("CSV_COMMENT",						10);
	define("CSV_END",								11);


	function ParseCSV(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false,
		$strSeparator = ',',
		$arrayCommentTokens = array(),
		$strQuote = '"',
		$strEscapedQuote = '""',
		$bNewLineInQuotedValue = false)
	{
		if (StringTrim($strCSV) == '')
		{
			return array();	
		}
		mb_internal_encoding("UTF-8");
		
		
		$strCSV = StringEncodeUTF8($strCSV);
		$strCSV = ReplaceString($strCSV,"\r\n","\n"); 
		$strCSV = ReplaceString($strCSV,"\r",''); 
		$strSeparator = StringEncodeUTF8($strSeparator);
		$strQuote = StringEncodeUTF8($strQuote);
		$strEscapedQuote = StringEncodeUTF8($strEscapedQuote);
		if (!is_array($arrayCommentTokens))
		{
			$arrayCommentTokens = array();
		}
		foreach ($arrayCommentTokens as &$strCommentToken)
		{
			$strCommentToken = StringEncodeUTF8($strCommentToken);
		}
		$strNewLine = "\n";

		$nLength = StringLength($strCSV);
		$bIsStringSingleByte = IsStringSingleByte($strCSV);
		$nLengthSeparator = StringLength($strSeparator);
		$nLengthQuote = StringLength($strQuote);
		$nLengthEscapedQuote = StringLength($strEscapedQuote);
		$nLengthNewLine = StringLength($strNewLine);


		if (	$bHeaderRow == true
			&&	$strEscapedQuote == ''
			&&	$nLengthQuote <= 1
			&&	ArrayCount($arrayCommentTokens) == 0
			&&	$bNewLineInQuotedValue == false)
		{
			// a header row exists, single character quotes, no comments, no newlines in quoted text, no escaped quotes?
			// Then we can use the built-in php function	
			
			$arrayCSV = array();
			$arrayRows = StringExplode($strCSV,$strNewLine);
			foreach ($arrayRows as $strRow)
			{
				$strRow = StringTrim($strRow);
				if ($strRow != '')
				{
					$arrayCSV[] = str_getcsv($strRow,$strSeparator,$strQuote);
				}
			}
			if (ArrayCount($arrayCSV) > 1)
			{
				$arrayRow1 = $arrayCSV[0];
				$nColumns = ArrayCount($arrayRow1);
				$bFirst = true;
				foreach ($arrayCSV as &$arrayRow)
				{
					if ($bFirst == true)
					{
						$bFirst = false;
						continue;
					}
					while (ArrayCount($arrayRow) < $nColumns)
					{
						$arrayRow[] = '';
					}
					$arrayRow = array_combine($arrayRow1, $arrayRow);
				}
/*
		    array_walk($arrayCSV, function(&$a) use ($arrayCSV)
		    {
		      $a = array_combine($arrayCSV[0], $a);
		    });
*/		    
		    if ($IncludeHeadersInResult == false)
		    {
		    	array_shift($arrayCSV); // remove column header
		    }
		  }
			return $arrayCSV;
		}








		
		$arrayRows = array();
		$arrayValues = array();
		$nPos = 0;
		$nStartPos = 0;
		$nPreviousState = CSV_INITIAL;
		$strNextValue = '';
		$strNextToken = '';
		
		$arrayNeedlesInitial = ArrayAppend(array($strSeparator,$strNewLine,$strQuote),$arrayCommentTokens);
		$arrayNeedlesUnquotedValue = ArrayAppend(array($strSeparator,$strNewLine),$arrayCommentTokens);
		$arrayNeedlesQuotedValue =  array($strQuote,$strEscapedQuote);
		if ($bNewLineInQuotedValue == false)
		{
			$arrayNeedlesQuotedValue[] = $strNewLine;
		}
		
		for ($nPos = 0; $nPos < $nLength; $nPos++)
		{
			if ($bIsStringSingleByte)
			{
				$cChar = $strCSV[$nPos];					
			}
			else
			{
				$cChar = StringSection($strCSV,$nPos,1);					
			}
						
			switch ($nPreviousState)
			{
			case CSV_INITIAL:
			case CSV_NEWLINE:
			case CSV_LEADINGWHITESPACE:
			case CSV_SEPARATOR:

				if ($cChar == ' '
					||$cChar == "\t")
				{
					$nPreviousState = CSV_LEADINGWHITESPACE;
					continue;
				}

				$strNextToken = '';
				$nPosNextToken = FindStringMultiple(
					$strCSV,
					$arrayNeedlesInitial,
					$strNextToken,
					false,
					$nPos);
					
				if ($nPosNextToken == -1)
				{
					$strNextValue = StringSection($strCSV,$nPos);
					$arrayValues[] = StringTrim($strNextValue);
					$strNextValue = "";
					$arrayRows[] = $arrayValues;
					$arrayValues = array();
					$nPreviousState = CSV_END;
					$nPos = $nLength;
					continue;	
				}

				if ($strNextToken == $strSeparator)
				{
					$strNextValue = StringSection($strCSV,$nPos,$nPosNextToken-$nPos);
					$arrayValues[] = StringTrim($strNextValue);
					$strNextValue = "";
					$nPreviousState = CSV_SEPARATOR;
					$nPos = $nPosNextToken;
					continue;
				}


				if ($strNextToken == $strNewLine)
				{
					if (	$nPosNextToken == $nPos
						&&	($nPreviousState == CSV_INITIAL || $nPreviousState == CSV_NEWLINE)
						)
					{
						$nPreviousState = CSV_NEWLINE;
						$nPos = $nPosNextToken;
						continue;
					}
					$strNextValue = StringSection($strCSV,$nPos,$nPosNextToken-$nPos);
					$arrayValues[] = StringTrim($strNextValue);
					$strNextValue = '';
					$arrayRows[] = $arrayValues;
					$arrayValues = array();
					$nPreviousState = CSV_NEWLINE;
					$nPos = $nPosNextToken;
					continue;
				}
			
				if ($strNextToken == $strQuote)
				{
					if ($nPos == $nPosNextToken)
					{
						$nPos += $nLengthQuote - 1;
						$nPreviousState = CSV_QUOTESTART;
						continue;
					}
					$strNextValue = StringSection($strCSV,$nPos,$nPosNextToken-$nPos);
					$nPos += $nPosNextToken - $nPos - 1;
					$nPreviousState = CSV_UNQUOTEDVALUE;
					continue;
				}

				// It must be a comment
				
				if ($nPos == $nPosNextToken)
				{
					$nPosNewLine = FindString($strCSV, $strNewLine, false, $nPos);
					if ($nPosNewLine != -1)
					{
						$nPos = $nPosNewLine;
						$nPreviousState = CSV_NEWLINE;
						continue;
					}
					$nPreviousState = CSV_END;
					$nPos = $nLength;
					continue;
				}
					
				$strNextValue = StringSection($strCSV,$nPos,$nPosNextToken-$nPos);
				$arrayValues[] = StringTrim($strNextValue);
				$strNextValue = "";
				$nPreviousState = CSV_COMMENT;

				break;
				
			
			case CSV_UNQUOTEDVALUE:

				$strNextToken = '';
				
				$nPosNextToken = FindStringMultiple(
					$strCSV,
					$arrayNeedlesUnquotedValue,
					$strNextToken,
					false,
					$nPos);
				
				if ($nPosNextToken > $nPos)
				{
					$strNextValue .= StringSection($strCSV,$nPos,$nPosNextToken-$nPos);

					if ($strNextToken == $strSeparator)
					{
						$nPos = $nPosNextToken;
						$arrayValues[] = StringTrim($strNextValue);
						$strNextValue = '';
						$nPreviousState = CSV_SEPARATOR;
						continue;
					}	
					if ($strNextToken == $strNewLine)
					{
						$nPos = $nPosNextToken;
						$arrayValues[] = StringTrim($strNextValue);
						$strNextValue = "";
						$arrayRows[] = $arrayValues;
						$arrayValues = array();
						$nPreviousState = CSV_NEWLINE;
						continue;
					}

					$arrayValues[] = StringTrim($strNextValue);
					$strNextValue = '';
					$nPreviousState = CSV_COMMENT;
					continue;
					
				}
				else
				{
					if ($nPosNextToken != -1)
					{
						if ($strNextToken == $strSeparator)
						{
							$arrayValues[] = StringTrim($strNextValue);
							$strNextValue = "";
							$nPreviousState = CSV_SEPARATOR;
							continue;
						}
						if ($strNextToken == $strNewLine)  
						{
							$arrayValues[] = StringTrim($strNextValue);
							$strNextValue = "";
							$arrayRows[] = $arrayValues;
							$arrayValues = array();
							$nPreviousState = CSV_NEWLINE;
							continue;
						}
						$arrayValues[] = StringTrim($strNextValue);
						$strNextValue = "";
						$nPreviousState = CSV_COMMENT;
						continue;
					}
				}
				$strNextValue .= $cChar;
				$nPreviousState = CSV_UNQUOTEDVALUE;
				break;
			
			
			case CSV_QUOTESTART:
			case CSV_ESCAPEDQUOTE:
			case CSV_QUOTEDVALUE:

				// TODO: optimize here!

				$strNextToken = "";
				
				$nPosNextToken = FindStringMultiple(
					$strCSV,
					$arrayNeedlesQuotedValue,
					$strNextToken,
					false,
					$nPos);	
							
				if ($nPosNextToken > $nPos)
				{
					$strNextValue .= StringSection($strCSV,$nPos,$nPosNextToken-$nPos);
					$nPos = $nPosNextToken-1;
					$nPreviousState = CSV_QUOTEDVALUE;
					continue;
				}


				$nPosEscapedQuote = FindString($strCSV, $strEscapedQuote, false, $nPos);
				if ($nPosEscapedQuote == $nPos)
				{
					$strNextValue .= $strQuote;
					$nPos += $nLengthEscapedQuote - 1;
					$nPreviousState = CSV_ESCAPEDQUOTE;
					continue;	
				}

				$nPosQuote = FindString($strCSV, $strQuote, false, $nPos);						
				if ($nPosQuote == $nPos)
				{
					$arrayValues[] = StringTrim($strNextValue);
					$strNextValue = '';

					$nPos += $nLengthQuote - 1;
					$nPreviousState = CSV_QUOTEEND;
					continue;
				}
				
				if ($bNewLineInQuotedValue == false)
				{
					$nPosNewLine = FindString($strCSV, $strNewLine, false, $nPos);
					if ($nPosNewLine == $nPos)
					{
						$arrayValues[] = StringTrim($strNextValue);
						$strNextValue = '';					
						$arrayRows[] = $arrayValues;
						$arrayValues = array();
						$nPos += $nLengthNewLine - 1;
						$nPreviousState = CSV_NEWLINE;
						continue;
					}
				}

				$strNextValue .= $cChar;
				$nPreviousState = CSV_QUOTEDVALUE;
				break;
				
			case CSV_QUOTEEND:
			case CSV_TRAILINGNOISE:

				if ($cChar == ' '
					||$cChar == "\t")
				{
					$nPreviousState = CSV_TRAILINGNOISE;
					continue;
				}

				$nPosSeparator = FindString($strCSV, $strSeparator, false, $nPos);
				if ($nPosSeparator == $nPos)
				{
					$nPreviousState = CSV_SEPARATOR;
					continue;
				}
				
				$nPosNewLine = FindString($strCSV, $strNewLine, false, $nPos);
				if ($nPosNewLine == $nPos)
				{
					$arrayRows[] = $arrayValues;
					$arrayValues = array();
					$nPreviousState = CSV_NEWLINE;
					continue;
				}
				
				$nPosCommentToken = FindStringMultiple($strCSV, $arrayCommentTokens, $strNextToken, false, $nPos);
				if ($nPosCommentToken == $nPos)
				{
					$nPreviousState = CSV_COMMENT;
					continue;
				}				

				$nPreviousState = CSV_TRAILINGNOISE;
				break;

			case CSV_COMMENT:

				$nPosNewLine = FindString($strCSV, $strNewLine, false, $nPos);
				if ($nPosNewLine != -1)
				{
					$arrayRows[] = $arrayValues;
					$arrayValues = array();
					
					$nPos = $nPosNewLine;
					$nPreviousState = CSV_NEWLINE;
					continue;
				}
				
				$nPos = $nLength; 
				$nPreviousState = CSV_END;
				break;

			
			default:
				FatalError("Invalid state in ExplodeCSV: $nState");
			}
			
		}
		if ($nPreviousState == CSV_SEPARATOR)
		{
			$arrayValues[] = "";
		}
		else if ($nPreviousState == CSV_UNQUOTEDVALUE)
		{
			$strNextValue = StringTrim($strNextValue);
			$arrayValues[] = $strNextValue;
		}
		if (ArrayCount($arrayValues) > 0)
		{
			$arrayRows[] = $arrayValues;
		}
		if (ArrayCount($arrayRows) == 0)
		{
			return array();	
		}

		$arrayResult = array();
		if ($bHeaderRow == true)
		{
			$arrayHeaders = $arrayRows[0];
			if ($IncludeHeadersInResult == true)
			{
				$arrayResult[] = $arrayHeaders;
			}
			$nHeaders = ArrayCount($arrayHeaders);
			$nLines = ArrayCount($arrayRows);
			for ($nLine = 1; $nLine < $nLines; $nLine++)
			{
				$arrayLineSource = ArrayGetValue($arrayRows,$nLine);
				$arrayLine = array();
				for ($nColumn = 0; $nColumn < $nHeaders; $nColumn++)
				{
					$arrayLine[ArrayGetValue($arrayHeaders,$nColumn)] = ArrayGetValue($arrayLineSource,$nColumn);
				}
				$arrayResult[] = $arrayLine; 
			}
		}
		else
		{
			$arrayResult = $arrayRows;
		}
		return $arrayResult;
	}




	function ParseCSV_AutoDetect(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		FatalError("ParseCSV_AutoDetect() is NOT IMPLEMENTED!");
	}

	function ParseCSV_Format(
		$strFormat,
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		switch ($strFormat)
		{
		case "rfc4180":
			return ParseCSV_RFC4180(
				$strCSV,
				$bHeaderRow,
				$IncludeHeadersInResult);
		case "excel":
			return ParseCSV_Excel(
				$strCSV,
				$bHeaderRow,
				$IncludeHeadersInResult);
		case "wdk":
			return ParseCSV_WDK(
				$strCSV,
				$bHeaderRow,
				$IncludeHeadersInResult);
		default:
			return ParseCSV_AutoDetect(
				$strCSV,
				$bHeaderRow,
				$IncludeHeadersInResult);
		}
	}

	function ParseCSV_RFC4180(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return ParseCSV(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult,
			',',
			array(),
			'"',
			'""',
			false);
	}

	function ParseCSV_Excel(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return ParseCSV_RFC4180(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult);
	}

	function ParseCSV_ExcelLegacy(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return ParseCSV(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult,
			';',
			array(),
			'"',
			'""');
	}


	function ParseCSV_WDK(
		$strCSV,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return ParseCSV(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult,
			';',
			array('#'),
			'"',
			'""',  
			true);
	}









	function FileReadCSV(
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false,
		$strSeparator = ',',
		$arrayCommentTokens = array(),
		$strQuote = '"',
		$strEscapedQuote = '""',
		$bNewLineInQuotedValue = false)
	{
		$strCSV = FileRead($strFile);
		if ($strCSV == false)
		{
			return false;
		}
		return ParseCSV(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult,
			$strSeparator,
			$arrayCommentTokens,
			$strQuote,
			$strEscapedQuote,
			$bNewLineInQuotedValue);
	}

	function FileReadCSV_AutoDetect(
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		$strCSV = FileRead($strFile);
		if ($strCSV == false)
		{
			return false;
		}
		return ParseCSV_AutoDetect(
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult);
	}

	function FileReadCSV_Format(
		$strFormat,
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		$strCSV = FileRead($strFile);
		if ($strCSV == false)
		{
			return false;
		}
		return ParseCSV_Format(
			$strFormat,
			$strCSV,
			$bHeaderRow,
			$IncludeHeadersInResult);

	}

	function FileReadCSV_RFC4180(
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return FileReadCSV_Format(
			"rfc4180",
			$strFile,
			$bHeaderRow,
			$IncludeHeadersInResult);
	}

	function FileReadCSV_Excel(
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return FileReadCSV_Format(
			"excel",
			$strFile,
			$bHeaderRow,
			$IncludeHeadersInResult);
	}

	function FileReadCSV_WDK(
		$strFile,
		$bHeaderRow = true,
		$IncludeHeadersInResult = false)
	{
		return FileReadCSV_Format(
			"wdk",
			$strFile,
			$bHeaderRow,
			$IncludeHeadersInResult);
	}















	
	
	
	function MakeCSV(
		$arrayList,
		$strSeparator = ",",
		$strQuote = '"',
		$strEscapedQuote = '""',
		$strEOL = "\r\n")
	{
		if (!is_array($arrayList))
		{
			return "";	
		}
		$nLines = ArrayCount($arrayList);
		if ($nLines == 0)
		{
			return "";	
		}
		$arrayColumns = array();
		foreach ($arrayList as $arrayRow)
		{
			if (is_array($arrayRow))
			{
				$arrayColumns = ArrayAppend($arrayColumns,array_keys($arrayRow));
			}
		}
		$arrayColumns = array_unique($arrayColumns);
		
		$strCSV = "";
		foreach ($arrayColumns as $strColumn)
		{
			if ($strCSV != "")
			{
				$strCSV .= $strSeparator;
			}
			$strCSV .= "\"".StringDecodeUTF8($strColumn)."\"";
		}
		
		foreach ($arrayList as $arrayRow)
		{
			if ($strCSV != "")
			{
				$strCSV .= $strEOL;
			}
			
			$strRow = "";
			foreach ($arrayColumns as $strColumn)
			{
				if ($strRow != "")
				{
					$strRow .= $strSeparator;
				}
				$strValue = ArrayGetValue($arrayRow,$strColumn);
				$strValue = ReplaceString($strValue,"\r\n","\n");
				$strValue = ReplaceString($strValue,"\r","\n");
				$strValue = ReplaceString($strValue,"\n",$strEOL);
				$strValue = StringDecodeUTF8($strValue);
				$strValue = ReplaceString($strValue,'"','""');
				$strRow .= '"'.$strValue.'"';
			}
			$strCSV .= $strRow;
		}
					
		return $strCSV;
	}
	
	function MakeCSV_Format(
		$strFormat,
		$arrayList)
	{
		switch ($strFormat)
		{
		case "rfc4180":
			return MakeCSV_RFC4180($arrayList);
		case "wdk":
			return MakeCSV_WDK($arrayList);
		case "excellegacy":
			return MakeCSV_ExcelLegacy($arrayList);
		case "excel":
		default:
			return MakeCSV_Excel($arrayList);
		}		
	}

	function MakeCSV_RFC4180(
		$arrayList)
	{
		return MakeCSV(
			$arrayList,
			",",
			'"',
			'""',
			"\r\n");
	}

	function MakeCSV_Excel(
		$arrayList)
	{
		return MakeCSV_RFC4180($arrayList);
	}

	function MakeCSV_ExcelLegacy(
		$arrayList)
	{
		return MakeCSV(
			$arrayList,
			";",  
			'"',
			'""',
			"\r\n");
	}


	function MakeCSV_WDK(
		$arrayList)
	{
		return MakeCSV(
			$arrayList,
			";",
			'"',
			'""',
			"\r\n");
	}


