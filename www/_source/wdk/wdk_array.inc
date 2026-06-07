<?php

	require_once('wdk_serialize.inc');

	function IsArray($array)
	{
		return is_array($array);
	}

	function ArrayCount($array)
	{
		if (!is_array($array))
		{
			return 0;	
		}
		return count($array);
		
	}

	function ArrayAppend($array1, $array2)
	{
		if (!is_array($array1))
		{
			$array1 = array();
		}
		if (!is_array($array2))
		{
			return $array1;
		}
		
		foreach ($array2 as $key => $value)
		{
			if (is_string($key))
			{
				$array1["$key"] = $value;
			}
			else
			{
				array_push($array1,$value);
			}
		}
		return $array1;
	}


	function CallbackArrayMergeSet($vValue1, $vValue2)
	{
		return $vValue2;
	}
	function CallbackArrayMergeAdd($vValue1, $vValue2)
	{
		if (is_numeric($vValue1) && is_numeric($vValue2))
		{
			return $vValue1 + $vValue2;
		}
		return $vValue2;
	}


	function ArrayMerge($array1, $array2, callable $CallbackArrayMerge = null)
	{
		$arrayResult = array();
		if (is_array($array1) == false)
		{
			if ($array1 == null)
			{
				return ArrayMerge($array2,array());
			}
			$array1 = array($array1);
		}
		if (is_array($array2) == false)
		{
			if ($array2 == null)
			{
				return ArrayMerge($array1,array());
			}
			$array2 = array($array2);
		}
		foreach($array1 as $vKey => $vValue)
		{
			if (is_numeric($vKey) == true)
			{
				$arrayResult[] = $vValue;	
			}
			else
			{
				$arrayResult[$vKey] = $vValue;
			}
		}
		foreach($array2 as $vKey => $vValue)
		{
			if (is_numeric($vKey) == true)
			{
				$arrayResult[] = $vValue;	
			}
			else
			{
				if ($CallbackArrayMerge == null)
				{
					$arrayResult[$vKey] = $vValue;
				}
				else
				{
					$arrayResult[$vKey] = $CallbackArrayMerge(ArrayGetValue($arrayResult,$vKey),$vValue);
				}
			}
		}
		return $arrayResult;
	}

	function ArrayMergeRecursive($array1, $array2, callable $CallbackArrayMerge = null)
	{
		$arrayResult = array();
		if (!is_array($array1) || !is_array($array2))
		{
			return ArrayMerge($array1, $array2, $CallbackArrayMerge);
		}
		foreach($array1 as $vKey => $vValue)
		{
			if (is_numeric($vKey) == true)
			{
				$arrayResult[] = $vValue;	
			}
			else
			{
				$arrayResult[$vKey] = $vValue;
			}
		}
		foreach($array2 as $vKey => $vValue)
		{
			if (is_numeric($vKey) == true)
			{
				$arrayResult[] = $vValue;	
			}
			else
			{
				if (is_array($vValue))
				{
					$arrayResult[$vKey] = ArrayMergeRecursive(ArrayGetValue($array1,$vKey),$vValue,$CallbackArrayMerge);	
				}
				else
				{
					if ($CallbackArrayMerge == null)
					{
						$arrayResult[$vKey] = $vValue;
					}
					else
					{
						$arrayResult[$vKey] = $CallbackArrayMerge(ArrayGetValue($arrayResult,$vKey),$vValue);
					}
				}
			}
		}
		return $arrayResult;
	}

	function ArrayMax($array1)
	{
		if (!is_array($array1))
		{
			return false;	
		}
		
		$valueMax = false;
		foreach ($array1 as $value) 
		{
			$value = floatval($value);
			if ($valueMax == false)
			{
				$valueMax = $value;	
			}
			else
			{
				$valueMax = max($valueMax,$value);	
			}
		}
		return $valueMax;
	}

	function ArrayMaxStringLength($array1)
	{
		if (!is_array($array1))
		{
			return false;	
		}
		
		$valueMax = false;
		foreach ($array1 as $value) 
		{
			$value = strval($value);
			if ($valueMax === false)
			{
				$valueMax = StringLength($value);
			}
			else
			{
				$valueMax = max($valueMax,StringLength($value));
			}
		}
		return $valueMax;
	}
 
 	function ArrayMinStringLength($array1)
	{
		if (!is_array($array1))
		{
			return false;	
		}
		
		$valueMin = false;
		foreach ($array1 as $value) 
		{
			$value = strval($value);
			$nStrLen = StringLength($value);
			if ($valueMin === false)
			{
				$valueMin = $nStrLen;	
			}
			else
			{
				$valueMin = min($valueMin,$nStrLen);	
			}
		}
		return $valueMin;
	}   

	function ArrayGetValue($array,$key1,$key2=null,$key3=null,$key4=null,$key5=null,$key6=null,$key7=null)
	{
		if (ArrayKeyExists($array,$key1) == false)	
		{
			return "";
		}
		if (($key2 == null || $key2 == "") && !($key2 === 0))
		{
			return $array[$key1];
		}
		return ArrayGetValue($array[$key1],$key2,$key3,$key4,$key5,$key6,$key7);
	}

	function ArrayGetValueByPosition($array,$nPosition = 0)
	{
		if (ArrayCount($array) <= $nPosition)
		{
			return false;	
		}
		reset($array);
		for ($nPos = 0; $nPos < $nPosition; $nPos++)
		{
			next($array);	
		}
		$vResult = current($array);
		return $vResult;
	}
	
	function ArrayGetLastValue($array)
	{
		if (IsArray($array) == false)
		{
			return false;
		}
		return end($array);
	}


	function ArraySetValue(&$array,$value,$key1,$key2=null,$key3=null,$key4=null,$key5=null,$key6=null,$key7=null)
	{
		if (!IsArray($array))
		{
			$array = array();
		}
		if ($key2 == null || $key2 == "")
		{
			$array[$key1] = $value;
			return;	
		}
		// if $key2 is set, $key1 must be an array
		ArraySetValue($array[$key1],$value,$key2,$key3,$key4,$key5,$key6,$key7);
	}


	function ArraySetValueRecursive($array,$newvalue,$key)
	{
		if (!IsArray($array))
		{
			return array();
		}
		foreach ($array as $currentkey => &$value)
		{
			$bMatch = false;
			if ($currentkey === $key)
			{
				$bMatch = true;
			}
			else
			{
				if (StringSection($key,0,1) == '/')
				{
					if (RegExpMatch($currentkey,$key) == true)
					{
						$bMatch = true;
					}
				}	
			}

			if ($bMatch == true)
			{
				$value = $newvalue;	
			}
			else if (is_array($value))
			{
				$value = ArraySetValueRecursive($value,$newvalue,$key);	
			}
		}
		return $array;
	}
	
	function ArrayMap($array,callable $CallbackMap,$arrayParams = array())
	{
		if (!IsArray($array))
		{
			return array();	
		}
		$arrayResult = array(); 
		foreach ($array as $key => $value)
		{
			$arrayResult[$key] = $CallbackMap($key,$value,$arrayParams);
		}
		return $arrayResult;
	}

	function ArrayMapRecursive($array,callable $CallbackMap,$arrayParams = array())
	{
		if (!IsArray($array))
		{
			return array();	
		}
		$arrayResult = array();
		foreach ($array as $key => $value)
		{
			if (IsArray($value))
			{
				$arrayResult[$key] = ArrayMapRecursive($value,$CallbackMap,$arrayParams);
			}
			else
			{
				$arrayResult[$key] = $CallbackMap($key,$value,$arrayParams);
			}
		}
		return $arrayResult;
	}

	
	function ArraySort($array,$bMaintainKeys = false, callable $CallbackCompare = null)
	{
		if (!IsArray($array))
		{
			return false;	
		}
		if ($bMaintainKeys == true)
		{
			if ($CallbackCompare == null)
			{
				$bResult = asort($array);
			}
			else
			{
				$bResult = uasort($array,$CallbackCompare);
			}
		}
		else
		{
			if ($CallbackCompare == null)
			{
				$bResult = sort($array);
			}
			else
			{
				$bResult = usort($array,$CallbackCompare);
			}
		}
		if ($bResult == false)
		{
			return false;	
		}
		return $array;
	}


	function ArraySortByKeys($array, callable $CallbackCompare = null)
	{
		if (!is_array($array))
		{
			return false;	
		}
		if ($CallbackCompare == null)
		{
			$bResult = ksort($array);
		}
		else
		{
			$bResult = uksort($array,$CallbackCompare);
		}
		if ($bResult == false)
		{
			return false;	
		}
		return $array;
	}


	function ArrayReverse($array,$bMaintainKeys = false)
	{
		if (!is_array($array))
		{
			return false;	
		}
		return array_reverse($array,$bMaintainKeys);	
	}

	function ArrayKeyFirst($arrayInput,$vKey)
	{
		if (!is_array($arrayInput))
		{
			return false;	
		}
		if (ArrayKeyExists($arrayInput,$vKey) == false)
		{
			return $arrayInput;
		}
		$arrayResult = array();
		if (is_int($vKey) == true)
		{
			$arrayResult[] = ArrayGetValue($arrayInput,$vKey);	
		}
		else
		{
			$arrayResult[$vKey] = ArrayGetValue($arrayInput,$vKey);	
		}
		$arrayResult = ArrayAppend($arrayResult,ArrayRemoveKey($arrayInput,$vKey));
		return $arrayResult;
	}

	function ArrayKeyLast($arrayInput,$vKey)
	{
		if (!is_array($arrayInput))
		{
			return false;	
		}
		if (ArrayKeyExists($arrayInput,$vKey) == false)
		{
			return $arrayInput;
		}
//		$value = ArrayGetValue($arrayInput,$vKey);
		$arrayResult = array();
		foreach ($arrayInput as $key => $vvalue)
		{
			if ($vKey != $key)
			{
				if (is_int($key) == true)
				{
					$arrayResult[] = ArrayGetValue($arrayInput,$key);	
				}
				else
				{
					$arrayResult[$key] = ArrayGetValue($arrayInput,$key);	
				}
			}	
		}

		if (is_int($vKey) == true)
		{
			$arrayResult[] = ArrayGetValue($arrayInput,$vKey);	
		}
		else
		{
			$arrayResult[$vKey] = ArrayGetValue($arrayInput,$vKey);	
		}
		return $arrayResult;
	}
	
	function ArrayKeysUpperCase($arrayInput,$bRecursive = false)
	{
		if (!is_array($arrayInput))
		{
			return $arrayInput;	
		}
		$arrayOutput = array();
		foreach ($arrayInput as $strKey => $value)
		{
			if ($bRecursive == true)
			{
				if (is_array($value))
				{
					$value = ArrayKeysUpperCaseRecursive($value);
				}
			}			
			$arrayOutput[StringUpperCase($strKey)] = $value;
		}	
		return $arrayOutput;
	}

	function ArrayKeysUpperCaseRecursive($arrayInput)
	{
		return ArrayKeysUpperCase($arrayInput,true);
	}   

	function ArrayKeysLowerCase($arrayInput,$bRecursive = false)
	{
		if (!is_array($arrayInput))
		{
			return $arrayInput;	
		}
		$arrayOutput = array();
		foreach ($arrayInput as $strKey => $value)
		{
			if ($bRecursive == true)
			{
				if (is_array($value))
				{
					$value = ArrayKeysLowerCaseRecursive($value);
				}
			}			
			$arrayOutput[StringLowerCase($strKey)] = $value;
		}	
		return $arrayOutput;
	}

	function ArrayKeysLowerCaseRecursive($arrayInput)
	{
		return ArrayKeysLowerCase($arrayInput,true);
	}   
	
	function ArrayKeyExists($arrayInput,$key)
	{
		if (!is_array($arrayInput))
		{
			return false;	
		}	
		return array_key_exists($key,$arrayInput);
	}
	
	function ArrayValueExists($arrayInput,$value,$bStrict=false)
	{
		if (is_array($arrayInput) == false)
		{
			return false;
		}
		return in_array($value,$arrayInput,$bStrict);
	}

	function ArrayCheckCharSet($arrayInput,$strCharSet,$bRecursive,$bKeys,$bValues)
	{
		if (StringLength($strCharSet) == 0)
		{
			return false;	
		}
		if (!is_array($arrayInput))
		{
			return false;	
		}
		foreach ($arrayInput as $key => $value)
		{
			if (is_array($value))
			{
				if ($bRecursive == true)
				{
					$bResult = ArrayCheckCharSet(
						$value,
						$strCharSet,
						$bRecursive,
						$bKeys,
						$bValues);
					if ($bResult == false)
					{
						return false;
					}
				}
			}
			else
			{
				if ($bValues == true)
				{
					if (StringCheckCharSet($value,$strCharSet) == false)
					{
						return false;	
					}
				}
			}
			if ($bKeys == true)
			{
				if (is_string($key))
				{
					if (StringCheckCharSet($key,$strCharSet) == false)
					{
						return false;	
					}
				}
			}
		}	
		return true;
	}

	function ArrayFilterByKeys($arraySource,$arrayKeys,$bIgnoreEmpty = false)
	{
		if (!is_array($arrayKeys))
		{
			return array();	
		}
		$arrayResult = array();		
		foreach ($arrayKeys as $key)
		{
			$value = ArrayGetValue($arraySource,$key);
			if (!$bIgnoreEmpty || $value != "")
			{
				$arrayResult[$key] = ArrayGetValue($arraySource,$key);
			}
		}
		return $arrayResult;	
	}

	function ArrayFilterListByKeys($arrayList,$arrayKeys,$bIgnoreEmpty = false)
	{
		if (!is_array($arrayKeys))
		{
			return array();	
		}
		if (!is_array($arrayList))
		{
			return array();	
		}
		$arrayResultList = array();		
		foreach ($arrayList as $arrayItem)
		{
			$arrayItemNew = ArrayFilterByKeys($arrayItem,$arrayKeys,$bIgnoreEmpty);
			if (is_array($arrayItemNew))
			{
				$arrayResultList[] = $arrayItemNew;	
			}
		}
		return $arrayResultList;	
	}
	
	function ArrayCheckKeysByWhiteList($arraySource,$arrayWhiteList,&$arrayForbiddenKeys = array())
	{
		if (!is_array($arraySource))
		{
			$arraySource = array();	
		}
		$bResult = true;
		foreach ($arraySource as $strKey => $value)
		{
			if (!ArrayValueExists($arrayWhiteList,$strKey))
			{
				$arrayForbiddenKeys[] = $strKey;
				$bResult = false;
			}					
		}
		return $bResult;
	}
	

	
	function ArrayRemoveValue($arraySource,$valueUnwanted)
	{
		$arrayResult = array();
		foreach ($arraySource as $key => $value)
		{
			if ($value != $valueUnwanted)
			{
				$arrayResult[$key] = $value;
			}
		}
		return $arrayResult;
	}
	
	function ArrayRemoveKey($arraySource,$keyUnwanted)
	{
		if (ArrayKeyExists($arraySource,$keyUnwanted) == true)
		{
			unset($arraySource[$keyUnwanted]);
		}
		return $arraySource;  
	} 
	
	function ArrayRemoveKeyRecursive($arraySource,$keyUnwanted)
	{
		
		if (ArrayKeyExists($arraySource,$keyUnwanted) == true)
		{
			unset($arraySource[$keyUnwanted]);
		}
		foreach ($arraySource as $key => &$value)
		{
			if (IsArray($value))
			{
				$value = ArrayRemoveKeyRecursive($value,$keyUnwanted);
			}
		}
		return $arraySource;  
	} 
	
	function ArrayTransformToTable($arraySource,$strHeaderKeys = "",$strHeaderValues = "")
	{
		if (!is_array($arraySource))
		{
			return false;	
		}
		$arrayResult = array();
		if ($strHeaderKeys != "" || $strHeaderValues != "")
		{
			$arrayResult[] = array($strHeaderKeys."",$strHeaderValues."");
		}
		foreach ($arraySource as $strKey => $strValue)
		{
			$arrayResult[] = array($strKey, $strValue);
		}
		return $arrayResult;
	}
	
	function ArrayTableRotate($arrayTableSource)
	{
		if (!is_array($arrayTableSource))
		{
			return array();	
		}	
		$arrayResult = array();
		reset($arrayTableSource);
		$arrayFirstRow = current($arrayTableSource);
		if (!is_array($arrayFirstRow))
		{
			return array();	
		}
		$nColumns = ArrayCount($arrayFirstRow);
		
		for ($nColumn = 0; $nColumn < $nColumns; $nColumn++)
		{
			$arrayNewRow = array();
			foreach($arrayTableSource as $arrayRow)
			{
				if (!is_array($arrayRow))
				{
					return array();
				}
				if (!ArrayKeyExists($arrayRow,$nColumn))
				{
					return array();	
				}
				$arrayNewRow[] = $arrayRow[$nColumn];
      }
			$arrayResult[] = $arrayNewRow;
    }
		return $arrayResult;
	}

	function ArraySerialize($arraySource)
	{
		if (!IsArray($arraySource))
		{
			return false;	
		}
		return ObjectSerialize($arraySource);
	}

	function ArrayUnserialize($vSerialized)
	{
		$arrayUnserialized = ObjectUnserialize($vSerialized);
		if (!IsArray($arrayUnserialized))
		{
			return false;	
		}
		return $arrayUnserialized;
	}
	
	
	function ArrayPack($arraySource)
	{
		$strSerialized = ArraySerialize($arraySource);
		if ($strSerialized == false)
		{
			return false;
		}
		return base64_encode($strSerialized);
	}

	function ArrayUnpack($strPacked)
	{
		$arrayUnpacked = ArrayUnserialize(base64_decode($strPacked));
		if (!IsArray($arrayUnpacked))
		{
			return false;	
		}
		return $arrayUnpacked;
	}
	
	function ArrayStrictCompare($array1,$array2)
	{
		if ($array1 != $array2)
		{
			return false;	
		}
		if (is_array($array1) == false || is_array($array2) == false)
		{
			return false;
		}
		if (ArrayCount($array1) != ArrayCount($array2))
		{
			return false;	
		}
		$nCount = ArrayCount($array1);
		reset($array1);
		reset($array2);
		for ($nPos = 0; $nPos  < $nCount; $nPos++)
		{
			if (current($array1) != current($array2))
			{
				return false;	
			}
			if (key($array1) != key($array2))
			{
				return false;	
			}
			next($array1);
			next($array2);
		}
		
		return true;
	}
	
	
	function ArrayContains($array1, $array2)
	{
		if (!IsArray($array1))
		{
			$array1 = [];	
		}
		if (!IsArray($array2))
		{
			$array2 = [];	
		}
		
   	foreach ($array2 as $vKey => $vValue)
		{
			if (is_numeric($vKey) == false) // associative key 
			{
				if (ArrayKeyExists($array1,$vKey) == false)
				{
					return false;	
				}
				if (IsArray(ArrayGetValue($array1,$vKey)) == true)
				{
					if (ArrayContains(ArrayGetValue($array1,$vKey),$vValue) == false)
					{
						return false;	
					}
				}
				else
				{
					if (ArrayGetValue($array1,$vKey) != $vValue)
					{
						return false;	
					}
				}
			}
			else  // indexed value
			{
				if (IsArray($vValue) == true)
				{
					$bContained = false;
					foreach ($array1 as $vKey1 => $vValue1)
					{
						if (IsArray($vValue1))
						{
							if (ArrayContains($vValue1,$vValue) == true)
							{
								$bContained = true;	
							}
						}
					}
					if ($bContained == false)
					{
						return false;	
					}
		
				}
				else
				{
					if (ArrayValueExists($array1,$vValue) == false)
					{
						return false;	
					}
				}
			}
		}
		return true;	
	}
	
	function ArrayUnique($arrayInput)
	{
		if (!is_array($arrayInput))
		{
			return false;	
		} 
		$ea = new CErrorAmnesty();
		$ea->Start();
		$arrayResult = array_unique($arrayInput);
		$ea->End();
		return $arrayResult;
	}
	
	function ArrayValues($arrayInput)
	{
		if (!is_array($arrayInput))
		{
			return array();	
		} 
		$ea = new CErrorAmnesty();
		$ea->Start();
		$arrayResult = array_values($arrayInput);
		$ea->End();
		return $arrayResult;
	}


