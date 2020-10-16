<?php

	define('CHARSET_ALPHA_LOWERCASE', 'abcdefghijklmnopqrstuvwxyz');
	define('CHARSET_ALPHA_UPPERCASE', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ');
	define('CHARSET_ALPHA', CHARSET_ALPHA_LOWERCASE.CHARSET_ALPHA_UPPERCASE);
	define('CHARSET_NUMERIC', '1234567890');

	define('CHARSET_ALPHANUMERIC_LOWERCASE', CHARSET_ALPHA_LOWERCASE.CHARSET_NUMERIC);
	define('CHARSET_ALPHANUMERIC_UPPERCASE', CHARSET_ALPHA_UPPERCASE.CHARSET_NUMERIC);
	define('CHARSET_ALPHANUMERIC', CHARSET_ALPHA.CHARSET_NUMERIC);

	define('CHARSET_SPECIAL', '!"§$%&/()=?{[]}\\+*~#\'-_,.;:<>|@ ');
	define('CHARSET_LINEBREAK', '\n\r');
	define('CHARSET_WHITESPACE', ' \t\n\r\0\x0B');
	
	define('CHARSET_SPECIALALPHA_GERMAN_LOWERCASE', 'ßäöü');
	define('CHARSET_SPECIALALPHA_GERMAN_UPPERCASE', 'ÄÖÜ');
	define('CHARSET_SPECIALALPHA_GERMAN', CHARSET_SPECIALALPHA_GERMAN_LOWERCASE.CHARSET_SPECIALALPHA_GERMAN_UPPERCASE);
	
	
	define('CHARSET_COMMAND', CHARSET_ALPHA_LOWERCASE);
	define('CHARSET_ACCESSCODE', CHARSET_ALPHANUMERIC);
	define('CHARSET_USERNAME', CHARSET_ALPHANUMERIC_LOWERCASE.'@.-');
	define('CHARSET_NAME', CHARSET_ALPHANUMERIC.'.-_/ ');
	define('CHARSET_TEXTID', CHARSET_ALPHANUMERIC.'-_/');
	define('CHARSET_TID', CHARSET_ALPHANUMERIC_UPPERCASE.'-_');
	define('CHARSET_IMAGEID', CHARSET_ALPHANUMERIC.'-_');
	define('CHARSET_FONTID', CHARSET_ALPHANUMERIC.'-.');
	define('CHARSET_VIDEOID', CHARSET_ALPHANUMERIC.'-.');
	define('CHARSET_ID', CHARSET_ALPHANUMERIC_LOWERCASE.'-');
	define('CHARSET_ID_LIST', CHARSET_ID.',*');
	define('CHARSET_TESTPATH', 'WDKROTLCAL'.CHARSET_ALPHANUMERIC_LOWERCASE.'-/:_');
	define('CHARSET_TESTID', CHARSET_ALPHANUMERIC_LOWERCASE.'-_');
 	define('CHARSET_TEXT', CHARSET_ALPHANUMERIC.CHARSET_SPECIALALPHA_GERMAN.CHARSET_SPECIAL.CHARSET_LINEBREAK);
	define('CHARSET_URL', CHARSET_ALPHANUMERIC.'/:%&()=~?+-_,.<>@#;!${}');
	define('CHARSET_HOSTNAME', CHARSET_ALPHANUMERIC.'.-');
	define('CHARSET_DOMAINNAME', CHARSET_ALPHANUMERIC.'-');
	define('CHARSET_TLD', CHARSET_ALPHANUMERIC.'.');
	define('CHARSET_FILEPATH', CHARSET_ALPHANUMERIC.'/:-_$.@');
	define('CHARSET_WEBSERVICENAME', CHARSET_ALPHANUMERIC_LOWERCASE.'/');
	define('CHARSET_ASSEMBLYID', CHARSET_ALPHANUMERIC_LOWERCASE.'-');
	define('CHARSET_EMAIL', CHARSET_ALPHANUMERIC.'@.-_');
	define('CHARSET_INTEGER', CHARSET_NUMERIC);
	define('CHARSET_FLOAT', CHARSET_NUMERIC.'.-');
	define('CHARSET_FLOAT_INTERNATIONAL', CHARSET_NUMERIC.'.,-');
	define('CHARSET_DATETIME', CHARSET_NUMERIC.'-: ');
	define('CHARSET_DATE', CHARSET_NUMERIC.'-');
	define('CHARSET_TIME', CHARSET_NUMERIC.':');
	define('CHARSET_DATE_INTERNATIONAL', CHARSET_NUMERIC.'-./ampAMP');
	define('CHARSET_BOOLEAN', '01truefalseonoff');
	define('CHARSET_HEX', 'abcdefABCDEF1234567890');
	define('CHARSET_PASSWORD', CHARSET_ALPHANUMERIC.'=&_+-:;,.%/\\@?!');
	define('CHARSET_DBFILTER', CHARSET_ALPHA_LOWERCASE);
	define('CHARSET_DBSORT', CHARSET_ALPHA_LOWERCASE);
	define('CHARSET_DBSORTORDER', 'ascdesc');
	define('CHARSET_DBTABLE', CHARSET_ALPHANUMERIC.'-_');
	define('CHARSET_PARAMETERNAME', CHARSET_ALPHANUMERIC_LOWERCASE.'-_');
	define('CHARSET_CRONTAB', CHARSET_NUMERIC.'/*,-');
	define('CHARSET_XMLTAG', CHARSET_ALPHANUMERIC_UPPERCASE.'_');
	define('CHARSET_ATTRIBUTENAME', CHARSET_ALPHANUMERIC_LOWERCASE.'_');
	define('CHARSET_COOKIE', CHARSET_ALPHANUMERIC.'_-');
	define('CHARSET_IPV4', CHARSET_NUMERIC.'.');
	define('CHARSET_VERSION', CHARSET_NUMERIC.'.');
	define('CHARSET_LANGUAGE', CHARSET_ALPHA_UPPERCASE);
	define('CHARSET_CURRENCY', CHARSET_ALPHA_UPPERCASE);
	define('CHARSET_TEL', CHARSET_NUMERIC.'+');
	define('CHARSET_PHONENUMBER', CHARSET_TEL.'-() ');
	define('CHARSET_PATH', CHARSET_ALPHANUMERIC.':/._-$ ');	
	

	define('INTEGER_MAX',0xFFFFFFF);
	
	define('OFFSET_MAX',65535);
	define('BLOCKSIZE_MAX',1024);
	
	define('EMAIL_MINLEN',6);
	define('EMAIL_MAXLEN',200);

	define('URL_MINLEN',11);
	define('URL_MAXLEN',512);


	function MakeID($vRaw)
	{
		if (is_array($vRaw))
		{
			$arraySorted = ArraySortByKeys($vRaw);
			$strID = '';
			foreach ($arraySorted as $vKey => $vValue)
			{
				$strID .= RenderValue($vKey);
				$strID .= RenderValue($vValue);
			}
		}
		else
		{
			$strID = strval($vRaw);	
		}
		$strID = StringLowerCase($strID);
		$strID = StringFilter($strID,CHARSET_TEXTID);
		return $strID;
	}


	function PrepareParameterArray()
	{
		$arrayGET = ArrayGetValue($GLOBALS,'_GET');
		$arrayPOST = ArrayGetValue($GLOBALS,'_POST');
		$arrayParams = array();
		if (is_array($arrayGET))
		{
			for (reset($arrayGET); $strKey = key($arrayGET); next($arrayGET))
			{
				$strValue = GetStringValue(pos($arrayGET));
				$arrayParams[$strKey] = StringEncodeUTF8($strValue);
			}
		}
		if (is_array($arrayPOST))
		{
			for (reset($arrayPOST); $strKey = key($arrayPOST); next($arrayPOST))
			{
				$strValue = GetStringValue(pos($arrayPOST));
				$arrayParams[$strKey] = StringEncodeUTF8($strValue);
			}
		}
		return SecureParameterArray($arrayParams);
	}
	
	
	

	class CParameterDefinition
	{
		private $m_arrayParamDefs;
		
		function __construct()
		{
			$this->m_arrayParamDefs = array();	
		}
		
		function GetParamDefData()
		{
			return $this->m_arrayParamDefs;
		}
		
		function AddParameterDefinition(
			$strName,
			$strType,
			$nMin,
			$nMax,
			$strCharset,
			$bAllowEmpty = false,
			$arrayValues = array())
		{
			$arrayParamDef = array();
			
			$strName = StringLowerCase($strName);
			
			$arrayParamDef['type'] = $strType;
			$arrayParamDef['min'] = $nMin;
			$arrayParamDef['max'] = $nMax;
			$arrayParamDef['charset'] = $strCharset;
			$arrayParamDef['allowempty'] = $bAllowEmpty;
			$arrayParamDef['enum'] = $arrayValues;
			
			$arrayParamDefOld = ArrayGetValue($this->m_arrayParamDefs,$strName);
			if (is_array($arrayParamDefOld))
			{
				if ($arrayParamDefOld != $arrayParamDef)
				{
					return false;
				}	
			}
			
			$this->m_arrayParamDefs[$strName] = $arrayParamDef;
			return true;
		}

		// NATIVE PARAMETER TYPES

		function AddParameterDefinitionEnum(
			$strName,
			$arrayValues)
		{
			if (!is_array($arrayValues))
			{
				$arrayValues = array();
			}
		
			$strName = StringLowerCase($strName);
			
			if (ArrayCount($arrayValues) == 0)
			{
				$nMin = 0;
				$nMax = 0;
				$strCharset = '';
			}
			else
			{
				$nMin = 0xFFFFFFFF;
				$nMax = 0;
				$strCharset = '';

				foreach ($arrayValues as $strValue)
				{
					$nMin = min($nMin,StringLength($strValue));
					$nMax = max($nMax,StringLength($strValue));
					$strCharset .= $strValue;
				}
			}
			
			return $this->AddParameterDefinition(
				$strName,
				'enum',
				$nMin,
				$nMax,
				$strCharset,
				false,
				$arrayValues);
		}


		function AddParameterDefinitionString(
			$strName,
			$nMin,
			$nMax,
			$strCharset,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'string',
				$nMin,
				$nMax,
				$strCharset,
				$bAllowEmpty);
		}

		
		function AddParameterDefinitionInteger(
			$strName,
			$nMin,
			$nMax,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'integer',
				$nMin,
				$nMax,
				CHARSET_INTEGER,
				$bAllowEmpty);
		}

		function AddParameterDefinitionFloat(
			$strName,
			$fMin,
			$fMax,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'float',
				$fMin,
				$fMax,
				CHARSET_FLOAT,
				$bAllowEmpty);
		}


		function AddParameterDefinitionBoolean($strName)
		{
			return $this->AddParameterDefinition(
				$strName,
				'boolean',
				0,
				0,
				CHARSET_BOOLEAN);
		}

		function AddParameterDefinitionDateTime(
			$strName,
			$strDateTimeMin,
			$strDateTimeMax,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'datetime',
				$strDateTimeMin,
				$strDateTimeMax,
				CHARSET_DATETIME,
				$bAllowEmpty);
		}

		function AddParameterDefinitionDate(
			$strName,
			$strDateMin,
			$strDateMax,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'date',
				$strDateMin,
				$strDateMax,
				CHARSET_DATE,
				$bAllowEmpty);
		}


		function AddParameterDefinitionEmail(
			$strName,
			$bAllowEmpty = false)
		{
			return $this->AddParameterDefinition(
				$strName,
				'email',
				EMAIL_MINLEN,
				EMAIL_MAXLEN,
				CHARSET_EMAIL,
				$bAllowEmpty);
		}



		// DERIVED PARAMETER TYPES

		function AddParameterDefinitionSHA1($strName)
		{
			return $this->AddParameterDefinition(
				$strName,
				'string',
				40,
				40,
				CHARSET_HEX);
		}

		function AddParameterDefinitionToken($strName)
		{
			return $this->AddParameterDefinition(
				$strName,
				'string',
				0,
				10,
				CHARSET_HEX);
		}

		function AddParameterDefinitionNumericID($strName)
		{
			return $this->AddParameterDefinitionInteger(
				$strName,
				0,
				INTEGER_MAX);
		}

		function AddParameterDefinitionID($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				32,
				CHARSET_ID);
		}

		function AddParameterDefinitionTextID($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				32,
				CHARSET_TEXTID);
		}

		function AddParameterDefinitionLongTextID($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				256,
				CHARSET_TEXTID);
		}
		
		function AddParameterDefinitionURL($strName, $bAllowEmpty = false)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				URL_MINLEN,
				URL_MAXLEN,
				CHARSET_URL,
				$bAllowEmpty);
		}


		function AddParameterDefinitionUsername($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				64,
				CHARSET_USERNAME);
		}
		
		
		function AddParameterDefinitionPassword($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				255,
				CHARSET_PASSWORD);
		}

		function AddParameterDefinitionTestPath($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				7,
				512,
				CHARSET_TESTPATH);
		}


		function AddParameterDefinitionCrontab($strName)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				1,
				200,
				CHARSET_CRONTAB);
		}
		
		function AddParameterDefinitionLanguage($strName,$bAllowEmpty = false)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				2,
				2,
				CHARSET_LANGUAGE,
				$bAllowEmpty);
		}

		function AddParameterDefinitionCurrency($strName,$bAllowEmpty = false)
		{
			return $this->AddParameterDefinitionString(
				$strName,
				3,
				3,
				CHARSET_CURRENCY,
				$bAllowEmpty);
		}

		

		// SPECIFIC PARAMETERS

		function AddParameterDefinition_userid()
		{
			return $this->AddParameterDefinitionNumericID('user_id');
		}

		
		function AddParameterDefinition_username()
		{
			return $this->AddParameterDefinitionUsername('user_name');
		}

		function AddParameterDefinition_sessionid()
		{
			return $this->AddParameterDefinitionSHA1('sessionid');
		}


		function AddParameterDefinition_language()
		{
			return $this->AddParameterDefinitionEnum(
				'language',
				GetAllLanguageIDs());
		}

		function AddParameterDefinition_languageAllowUnset()
		{
			$arrayEnum = GetAllLanguageIDs();
			array_push($arrayEnum,'');
			return $this->AddParameterDefinitionEnum(
				'language',
				$arrayEnum);
		}


		function AddParameterDefinition_country()
		{
			return $this->AddParameterDefinitionEnum(
				'country',
				GetAllCountryIDs());
		}

		function AddParameterDefinition_countryAllowUnset()
		{
			$arrayEnum = GetAllCountryIDs();
			array_push($arrayEnum,'');
			return $this->AddParameterDefinitionEnum(
				'country',
				$arrayEnum);
		}

		function AddParameterDefinition_timezone()
		{
			return $this->AddParameterDefinitionEnum(
				'timezone',
				GetAllTimezoneIDs());
		}


		function AddParameterDefinition_password()
		{
			return $this->AddParameterDefinitionPassword(
				'password');
		}

		function AddParameterDefinition_passwordhash()
		{
			return $this->AddParameterDefinitionSHA1(
				'passwordhash');
		}

		function AddParameterDefinition_passwordhashseed()
		{
			return $this->AddParameterDefinitionSHA1(
				'passwordhash_seed');
		}


		function AddParameterDefinition_authenticationpayload()
		{
			return $this->AddParameterDefinitionString(
				'authenticationpayload',
				1,
				255,
				CHARSET_HEX . CHARSET_PASSWORD);
		}
		
		function AddParameterDefinition_offset()
		{
			return $this->AddParameterDefinitionInteger(
				'offset',
				0,
				OFFSET_MAX);
		}

		function AddParameterDefinition_blocksize()
		{
			return $this->AddParameterDefinitionInteger(
				'blocksize',
				0,
				BLOCKSIZE_MAX);
		}


		function AddParameterDefinition_trace()
		{
			return $this->AddParameterDefinitionBoolean(
				'trace');
		}
	
		function AddParameterDefinition_testpath()
		{
			return $this->AddParameterDefinitionTestPath(
				'test_path');
		}
		
		function AddParameterDefinitions_Filter(
			$arrayFilterOptions,
			$strPrefix,
			$nFilterParams = 10)
		{
			if ($strPrefix != '')
			{
				$strPrefix .= '_';	
			}
			
			$arrayFilterOperators = array(
				'<',
				'>',
				'<=',
				'>=',
				'=',
				'!=',
				'contains',
				'like',
				'in',
				'');
				
			if (is_array($arrayFilterOptions))
			{
				for ($nIndex = 1; $nIndex <= $nFilterParams; $nIndex++)
				{
					$bResult = $this->AddParameterDefinitionEnum(
						$strPrefix.'filter'.$nIndex,
						$arrayFilterOptions);
					if ($bResult == false)
					{
						return false;	
					}
					$bResult = $this->AddParameterDefinitionEnum(
						$strPrefix.'filter'.$nIndex.'_operator',
						$arrayFilterOperators);
					if ($bResult == false)
					{
						return false;	
					}
					$bResult = $this->AddParameterDefinitionString(
						$strPrefix.'filter'.$nIndex.'_value',
						0,255,
						CHARSET_TEXT);
					if ($bResult == false)
					{
						return false;	
					}
				}
			}
			return true;
		}

		function AddParameterDefinitions_Sort(
			$arraySortOptions,
			$strPrefix)
		{
			if ($strPrefix != '')
			{
				$strPrefix .= '_';	
			}
			if (is_array($arraySortOptions))
			{
				$nSortOptions = ArrayCount($arraySortOptions);
				for ($nIndex = 1; $nIndex <= $nSortOptions; $nIndex++)
				{
					$bResult = $this->AddParameterDefinitionEnum(
						$strPrefix.'sort'.$nIndex.'',
						$arraySortOptions);
					if ($bResult == false)
					{
						return false;	
					}
					$bResult = $this->AddParameterDefinitionEnum(
						$strPrefix.'sort'.$nIndex.'_order',
						array('asc','desc'));
					if ($bResult == false)
					{
						return false;	
					}
				}
			}
			return true;			
		}

		
		function IsValid()
		{
			foreach ($this->m_arrayParamDefs as $arrayParamDef)
			{
				if (!is_array($arrayParamDef))
				{
					return false;
				}
				$strType = ArrayGetValue($arrayParamDef,'type');
				switch ($strType)
				{
					case 'date':
						$strDateMin = ArrayGetValue($arrayParamDef,'min');
						if ($strDateMin != '')
						{
							if (IsValidDate($strDateMin) != true)	
							{
								return false;	
							}
						}
						$strDateMax = ArrayGetValue($arrayParamDef,'max');
						if ($strDateMax != '')
						{
							if (IsValidDate($strDateMax) != true)	
							{
								return false;	
							}
						}
						
						if (($strDateMax != '') && ($strDateMin != ''))
						{
							if (!(strcmp($strDateMin,$strDateMax) < 0))	
							{
								return false;	
							}
						}
						
						
						break;
						
					case 'datetime':
						$strDateTimeMin = ArrayGetValue($arrayParamDef,'min');
						if ($strDateTimeMin != '')
						{
							if (IsValidDateTime($strDateTimeMin) != true)	
							{
								return false;	
							}
						}
						$strDateTimeMax = ArrayGetValue($arrayParamDef,'max');
						if ($strDateTimeMax != '')
						{
							if (IsValidDateTime($strDateTimeMax) != true)	
							{
								return false;	
							}
						}
						
						if (($strDateTimeMax != '') && ($strDateTimeMin != ''))
						{
							if (!(strcmp($strDateTimeMin,$strDateTimeMax) < 0))	
							{
								return false;	
							}
						}						
						break;
						
					case 'float':
						$fMin = $arrayParamDef['min'];
						if (!is_float($fMin))
						{
							return false;	
						}
						$fMax = $arrayParamDef['max'];
						if (!is_float($fMax))
						{
							return false;	
						}
						if ($fMin > $fMax)
						{
							return false;
						}
						break;					
					default:
						break;	
				}
			}
			return true;	
		}
		
		function CheckParameters($arrayParams,&$arrayErrors,$bIgnoreUnknown = false)
		{
			$arrayErrors = array();
			foreach ($arrayParams as $strParam => $strValue)
			{
				$strParam = StringLowerCase($strParam);

				if (!ArrayKeyExists($this->m_arrayParamDefs,$strParam))
				{
					if ($bIgnoreUnknown == false)
					{				
						$arrayErrors[$strParam] = 'PARAMETER_UNKNOWN';
					}
					continue;
				} 
				
				$arrayParamDef = $this->m_arrayParamDefs[$strParam];
				
				$strCharsetDef = $arrayParamDef['charset'];
				$nMin = intval($arrayParamDef['min']);
				$nMax = intval($arrayParamDef['max']);
				$bAllowEmpty = intval($arrayParamDef['allowempty']);

				if ($bAllowEmpty && ($strValue == ''))
				{
					continue;	
				}
        		
	  		if ($strCharsetDef != '')
	  		{
					if (StringCheckCharSet($strValue,$strCharsetDef) != true)
					{
						if (StringCheckCharSet($strValue,u($strCharsetDef)) != true)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID_CHARACTERS';	
							continue;
						}
					}
				}
				
				$strTypeDef = $arrayParamDef['type'];
				switch ($strTypeDef)
				{
					case 'integer':
						$nValue = intval($strValue);
						if ($nValue > $nMax)
						{ 
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_MAX';
							break;
						}
						if ($nValue < $nMin)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_MIN';
							break;
						}
						
						break;
						
					case 'float':
						$fMin = floatval($arrayParamDef['min']);
						$fMax = floatval($arrayParamDef['max']);
						
						if (!IsFloatString($strValue))
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
							break;
						}
						
						$fValue = floatval($strValue);
						if (strval($arrayParamDef['max']) != '')
						{
							if ($fValue > $fMax)
							{
								$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_MAX';
								break;
							}
						}
						if ($fValue < $fMin)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_MIN';
							break;
						}
						break;						
					
					case 'enum':
						$arrayValues = $arrayParamDef['enum'];
						if (!is_array($arrayValues))
						{
							$arrayValues = array();
						}
						
						if (!ArrayValueExists($arrayValues,$strValue))
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
							break;
						}
						break;
						
					case 'string':
						$nLength = StringLength($strValue);
						if ($nLength > $nMax)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_LONG';
							break;
						}
						if ($nLength < $nMin)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_SHORT';
							break;
						}
						break;

					case 'email':
						$nLength = StringLength($strValue);
						if ($nLength > $nMax)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_LONG';
							break;
						}
						if ($nLength < $nMin)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_SHORT';
							break;
						}
						if (IsValidEmail($strValue) == false)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
							break;
						}
						break;

					case 'datetime':
						if (IsValidDateTime($strValue) == false)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
							break;	
						}

						$strMin = $arrayParamDef['min'];
						$strMax = $arrayParamDef['max'];

						if ($strMin != '')
						{
							if (!(strcmp($strMin,$strValue) <= 0))
							{
								$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_TOOEARLY';
								break;
							}
						}
						if ($strMax != '')
						{
							if (!(strcmp($strValue,$strMax) <= 0))
							{
								$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_TOOLATE';
								break;
							}
						}
						break;

					case 'date':
						if (IsValidDate($strValue) == false)
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
							break;	
						}
						
						$strMin = $arrayParamDef['min'];
						$strMax = $arrayParamDef['max'];

						if ($strMin != '')
						{
							if (!(strcmp($strMin,$strValue) < 0))
							{
								$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_TOOEARLY';
								break;
							}
						}
						if ($strMax != '')
						{
							if (!(strcmp($strValue,$strMax) < 0))
							{
								$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_TOOLATE';
								break;
							}
						}
						break;

					case 'boolean':
						if (	$strValue != ''
							&& $strValue != '1'
							&& $strValue != '0'
							&& $strValue != 'true'
							&& $strValue != 'false'
							&& $strValue != 'on'
							&& $strValue != 'off')
						{
							$arrayErrors[$strParam] = 'PARAMETER_'.StringUpperCase($strParam).'_INVALID';
						}
						break;

						
					default:
						break;	
				}
			}
			return ArrayCount($arrayErrors);
		}
		
	}




