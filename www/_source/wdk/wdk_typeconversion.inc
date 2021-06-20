<?php

	function GetIntegerValue($value)
	{
		if (is_float($value))
		{
			return round($value);
		}
		if (is_string($value))
		{
			if ($value == '')
			{
				return 0;	
			}
			$value = StringRemoveControlChars($value);
			$value = StringUpperCase($value);
			if (StringSection($value,-2) == 'MB')
			{
				return round($value) * 1024 * 1024;
			}
			if (StringSection($value,-2) == 'TB')
			{
				return round($value) * 1024 * 1024 * 1024;
			}
			if (StringSection($value,-2) == 'KB')
			{
				return round($value) * 1024;
			}
			if (StringSection($value,-2) == 'BN')
			{
				return round($value) * 1000000000;
			}
			if (StringSection($value,-1) == 'M')
			{
				return round($value) * 1000000;
			}
			if (StringSection($value,-1) == 'T')
			{
				return round($value) * 1000;
			}
		}
		return intval($value);
	}
	
	function GetStringValue($value)
	{
		if (is_array($value))
		{
			return '';	
		}
		if ($value === false)
		{
			return '0';	
		}

		return ''.$value;
	}


	function GetBoolValue($value)
	{
		if (is_array($value))
		{
			if (ArrayCount($value) == 0)
				return false;
			return true;
		}
		if ($value == '')
			return false;
		if ($value == '1')
			return true;
		if ($value == '0')
			return false;
		if (StringLowerCase($value) == 'true')
			return true;
		if (StringLowerCase($value) == 'false')
			return false;
		if ($value == true)
			return true;
		return false;
	}
	
	function IsBoolString($value)
	{
		if (!is_string($value))
		{
			return false;
		}
		if (is_array($value))
		{
			return false;
		}
		if ($value == '')
			return false;
			
		if ($value == '1')
			return true;
		if ($value == '0')
			return true;
		if (StringLowerCase($value) == 'true')
			return true;
		if (StringLowerCase($value) == 'false')
			return true;
		return false;
	}


	function GetFloatValue($value)
	{
		if (is_float($value))
		{
			return $value;	
		}
		$strValue = strval($value);
    $nPosDot = strrpos($strValue, '.');
    $nPosComma = strrpos($strValue, ',');
    $nPosSeparator =
    	(($nPosDot > $nPosComma) && $nPosDot)
    	?	$nPosDot : 
				((($nPosComma > $nPosDot) && $nPosComma)
				? $nPosComma :
					false);
    if (!$nPosSeparator)
    {
    	$strValue = StringFilter($strValue,CHARSET_NUMERIC.'-');
			return floatval($strValue);
    }
    return floatval(
        StringFilter(substr($strValue, 0, $nPosSeparator),CHARSET_NUMERIC.'-') . '.' .
        StringFilter(substr($strValue, $nPosSeparator+1, strlen($strValue)),CHARSET_NUMERIC)
    	);		
	}

	
	function IsFloatString($value)
	{
		if (!is_string($value))
		{
			return false;	
		}
		
		//$strRegEx = '/^([+-]?(((\d+(\.)?)|(\d*\.\d+))([eE][+-]?\d+)?))$/';
		//$strRegEx = '/^([+/-]?((([0-9]+(\.)?)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?))$/';
		$strRegEx = '/^([+-]?(((\d+(\.)?)|(\d*\.\d+))([eE][+-]?\d+)?))$/';
	
		if (RegExpMatch($value,$strRegEx) == false)
		{
			return false;	
		}
		return true;
	}
	
	function IsUnsignedIntegerString($value)
	{
		if (!is_string($value))
		{
			return false;	
		}
		if ($value == '')
		{
			return false;	
		}
		if (StringCheckCharSet($value,CHARSET_INTEGER) != true)
		{
			return false;
		}
		return true;
	}
	
	function RenderBool($value,$bUppercase = false)
	{
		if ($bUppercase == true)
		{
			return ((GetBoolValue($value))?('TRUE'):('FALSE'));
		}
		return ((GetBoolValue($value))?('true'):('false'));
	}
	
	
	function RenderHex($strRaw)
	{
		$strHex = '';
		$nLength = strlen(GetStringValue($strRaw));
    $i = 0;
		for ($i = 0; $i < $nLength; $i++)
    {
			$strHex .= sprintf('%02x', ord($strRaw{$i}));
    }
    return $strHex;
	}

	function RenderInteger($value)
	{
		$value = GetIntegerValue($value);
		return ''.strval($value);
	}	
	
	
	function RenderValue($value)
	{
		if (is_string($value))
		{
			return "\"$value\"";	
		}	
		if (is_array($value))
		{
			return ArrayRender($value);
		}
		if (is_object($value))
		{
			return ArrayRender($value);	
		}
		if ($value === true || $value === false)
		{
			return RenderBool($value);	
		}
		return "$value";
	}
	

	
