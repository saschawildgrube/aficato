<?php

	function IsStringUTF8($strString)
	{
		$strEncoding = mb_detect_encoding($strString."XXXXX",'UTF-8, ISO-8859-1', true);
		if (StringUpperCase($strEncoding) != 'UTF-8') 
		{
			return false;
		}
		return true;
	}

	function IsStringSingleByte($strString)
	{
		$nLengthMB = mb_strlen($strString,"UTF-8");	
		$nLengthRaw = strlen($strString);	
		if ($nLengthMB == $nLengthRaw)
		{
			return true;	
		}
		return false;
	}


	function u($str)
	{
		return StringEncodeUTF8($str);	
	}

	$g_arrayByteMap_StringEncodeUTF8 = null;
	$g_strGoodChars_StringEncodeUTF8 = null;
	function StringEncodeUTF8($str)
	{
		if (IsStringUTF8($str) == true)
		{
			return $str;
		}
		
		// Step 1: Create the byte map
		global $g_arrayByteMap_StringEncodeUTF8;
		global $g_strGoodChars_StringEncodeUTF8;
		if (!is_array($g_arrayByteMap_StringEncodeUTF8))
		{
			$g_arrayByteMap_StringEncodeUTF8 = array();
			for($nChar = 128; $nChar < 256; $nChar++)
			{
    			$g_arrayByteMap_StringEncodeUTF8[chr($nChar)]=utf8_encode(chr($nChar));
 			}
 			$arrayCP1252Map = array(
				"\x80" => "\xE2\x82\xAC",  // EURO SIGN
				"\x82" => "\xE2\x80\x9A",  // SINGLE LOW-9 QUOTATION MARK
				"\x83" => "\xC6\x92",      // LATIN SMALL LETTER F WITH HOOK
				"\x84" => "\xE2\x80\x9E",  // DOUBLE LOW-9 QUOTATION MARK
				"\x85" => "\xE2\x80\xA6",  // HORIZONTAL ELLIPSIS
				"\x86" => "\xE2\x80\xA0",  // DAGGER
				"\x87" => "\xE2\x80\xA1",  // DOUBLE DAGGER
				"\x88" => "\xCB\x86",      // MODIFIER LETTER CIRCUMFLEX ACCENT
				"\x89" => "\xE2\x80\xB0",  // PER MILLE SIGN
				"\x8A" => "\xC5\xA0",      // LATIN CAPITAL LETTER S WITH CARON
				"\x8B" => "\xE2\x80\xB9",  // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
				"\x8C" => "\xC5\x92",      // LATIN CAPITAL LIGATURE OE
				"\x8E" => "\xC5\xBD",      // LATIN CAPITAL LETTER Z WITH CARON
				"\x91" => "\xE2\x80\x98",  // LEFT SINGLE QUOTATION MARK
				"\x92" => "\xE2\x80\x99",  // RIGHT SINGLE QUOTATION MARK
				"\x93" => "\xE2\x80\x9C",  // LEFT DOUBLE QUOTATION MARK
				"\x94" => "\xE2\x80\x9D",  // RIGHT DOUBLE QUOTATION MARK
				"\x95" => "\xE2\x80\xA2",  // BULLET
				"\x96" => "\xE2\x80\x93",  // EN DASH
				"\x97" => "\xE2\x80\x94",  // EM DASH
				"\x98" => "\xCB\x9C",      // SMALL TILDE
				"\x99" => "\xE2\x84\xA2",  // TRADE MARK SIGN
				"\x9A" => "\xC5\xA1",      // LATIN SMALL LETTER S WITH CARON
				"\x9B" => "\xE2\x80\xBA",  // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
				"\x9C" => "\xC5\x93",      // LATIN SMALL LIGATURE OE
				"\x9E" => "\xC5\xBE",      // LATIN SMALL LETTER Z WITH CARON
				"\x9F" => "\xC5\xB8"       // LATIN CAPITAL LETTER Y WITH DIAERESIS
			);
  			
			foreach($arrayCP1252Map as $strChar => $strMapping)
			{
  			$g_arrayByteMap_StringEncodeUTF8[$strChar] = $strMapping;
			}
  			
 			$strAsciiChar	= '[\x00-\x7F]';
			$strContByte	= '[\x80-\xBF]';
			$strUTF8_2		= '[\xC0-\xDF]'.$strContByte;
			$strUTF8_3		= '[\xE0-\xEF]'.$strContByte.'{2}';
			$strUTF8_4		= '[\xF0-\xF7]'.$strContByte.'{3}';
			$strUTF8_5		= '[\xF8-\xFB]'.$strContByte.'{4}';
			$g_strGoodChars_StringEncodeUTF8	= "@^($strAsciiChar+|$strUTF8_2|$strUTF8_3|$strUTF8_4|$strUTF8_5)(.*)$@s";
 		}

		// Step 2: Do the conversion
		$strOut = '';
		$char = '';
		$strRest = '';
		while( (strlen($str)) > 0)
		{
  		if (1 == preg_match($g_strGoodChars_StringEncodeUTF8,$str,$arrayMatch))
  		{
    		$char = $arrayMatch[1];
    		$strRest = $arrayMatch[2];
    		$strOut .= $char;
  		}
  		else if (1 == preg_match('@^(.)(.*)$@s',$str,$arrayMatch))
  		{
	      $char = $arrayMatch[1];
	      $strRest = $arrayMatch[2];
	      $strOut .= $g_arrayByteMap_StringEncodeUTF8[$char];
    	}
 	 		$str = $strRest;
		}
  	return $strOut;
	}
	
	function StringDecodeUTF8($str)
	{
		if (!is_string($str))
		{
			return '';	
		}
		return utf8_decode($str);	
	}
	
	function IsFileUTF8BOM($strFilePath)
	{
		$strFileContents = FileReadBinary($strFilePath);
		if ($strFileContents == false)
		{
			return false;	
		}	
		if (substr($strFileContents,0,3) != "\xEF\xBB\xBF")
		{
			return false;
		}
		return true;
	}


