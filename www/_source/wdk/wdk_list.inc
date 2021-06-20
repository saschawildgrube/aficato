<?php


	class CList
	{
		private $m_arrayList;
		
		function __construct()
		{
			$this->m_arrayList = array();
		}
		
		function GetListArray()
		{
			$arrayList = $this->m_arrayList;
			if (!is_array($arrayList))
			{
				$arrayList = array();
			}
			return $arrayList;	
		}
		
		function SetListArray($arrayList)
		{
			if (!is_array($arrayList))
			{
				$arrayList = array();
			}
			$this->m_arrayList = $arrayList;
		}
		
		function GetCount()
		{
			return ArrayCount($this->GetListArray());
		}
		
	
		function Filter($arrayFilterOptions)
		{
			$arrayList = $this->GetListArray();
			$arrayListFiltered = array();

			if (!is_array($arrayFilterOptions)) 
			{
				return false;	
			}
			if (ArrayCount($arrayFilterOptions) == 0)
			{
				return true;	
			}

			$arrayListFiltered = array();

			foreach ($arrayList as $arrayElement)
			{
				$bAddToResult = true;
				foreach ($arrayFilterOptions as $arrayOption)
				{
					if ($bAddToResult == false)
					{
						continue;	
					}
					if (!is_array($arrayOption))	
					{
						return false;	
					}
					$strColumn = ArrayGetValue($arrayOption,"field");
					// This is behaviour by design. If the field does not exist, it will not be considered. It will not just be assumed the fields is "" or 0.
					if (!ArrayKeyExists($arrayElement,$strColumn))
					{
						continue;	
					}
					$strValue = ArrayGetValue($arrayOption,"value");
					$strOperator = ArrayGetValue($arrayOption,"operator");

					switch ($strOperator)
					{
						case "=":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) == $strValue) != true)
							{
								$bAddToResult = false;
							}
							break;
						}
						case "!=":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) != $strValue) != true)
							{
								$bAddToResult = false;
							}
							break;	
						}
						case "<":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) < $strValue) != true)
							{
								$bAddToResult = false;
							}							
							break;	
						}
						case ">":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) > $strValue) != true)
							{
								$bAddToResult = false;
							}
							break;	
						}
						case "<=":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) <= $strValue) != true)
							{
								$bAddToResult = false;
							}
							break;	
						}
						case ">=":
						{
							if ((ArrayGetValue($arrayElement,$strColumn) >= $strValue) != true)
							{
								$bAddToResult = false;
							}
							break;	
						}
						case "contains":
						{
							if ((FindString(StringLowerCase(ArrayGetValue($arrayElement,$strColumn)),StringLowerCase($strValue)) != -1) != true)
							{
								$bAddToResult = false;
							}							
							break;	
						}
						case "in":
						{
							$strElementValue = ArrayGetValue($arrayElement,$strColumn);
							$bAddToResult = false;
          				$arrayValues = explode(",",$strValue);
          				foreach ($arrayValues as $strValue2)
          				{
          					if ($strElementValue == $strValue2)
          					{
          						$bAddToResult = true;
          						break;
          					}	
          				}
          				break;
						}
						case "like":
							// mysql like search is not (yet) supported.
						default:
							return false;	
					}
				}
				if ($bAddToResult == true)
				{
					array_push($arrayListFiltered,$arrayElement);
				}
				
			}
			
			
			$this->SetListArray($arrayListFiltered);
			
			return true;	
		}
		
		
		function Sort($arraySortOptions)
		{
			if (!is_array($arraySortOptions)) 
			{
				return false;	
			}
			if (ArrayCount($arraySortOptions) == 0)
			{
				return true;	
			}
 			foreach ($arraySortOptions as $strKey => $strSortOrder)
 			{
 				// Protection against code injection: Without " and ; a code injection will not be possible.
				if (FindString($strKey,"\"") != -1)
				{
					return false;	
				}
				if (FindString($strKey,";") != -1)
				{
					return false;	
				}

				if ($strSortOrder != "asc" && $strSortOrder != "desc")
				{
					return false;	
				}
			}

			
			$arrayList = $this->GetListArray();
			if (ArrayCount($arrayList) <= 1)
			{
				return true;
			}
			
			// 1. Convert list to columns
			$arrayColumns = array();
			foreach ($arrayList as $key => $arrayRow)
			{ 
    			foreach ($arraySortOptions as $strKey => $strSortOrder)
    			{
    				// StringLowerCase() will make the code injection even more difficult.
	  				$arrayColumns[$strKey][$key] = StringLowerCase(ArrayGetValue($arrayRow,$strKey));
				}
			} 
			
			// Store original keys 
  		$arrayKeys = array_keys($arrayList); 
  
  		// Create Sort Expression 
			$i = 0; 
			$strParameter = "";
			foreach ($arraySortOptions as $strKey => $strSortOrder)
			{ 
				if (ArrayKeyExists($arrayColumns,$strKey))
				{
					$i++; 
    			if($i > 1)
    			{
    				$strParameter .= ",";
    			} 
    			$strParameter .= '$arrayColumns["'.$strKey.'"]'; 
    			$strParameter .= ',SORT_'.StringUpperCase($strSortOrder);
    		}
			}
			if ($i == 0)
			{
				return false;
			}
			$strParameter.=',$arrayKeys'; 
			
			// Call Sort Function
			$strSortFunctionExpression = 'array_multisort('.$strParameter.');'; 
			eval($strSortFunctionExpression); 
			
			// Rebuild List Array
			$arrayListSorted = array();
			foreach($arrayKeys as $key)
			{ 
  			array_push($arrayListSorted,$arrayList[$key]); 
			}
			
			$this->SetListArray($arrayListSorted);
			
			return true;
		}
		
		
		function Block($nBlockSize,$nOffset)
		{
			$arrayList = $this->GetListArray();
			$arrayListBlock = array();

			$nLowerLimit = $nOffset * $nBlockSize;
			$nUpperLimit = (($nOffset+1) * $nBlockSize);

			$arrayListBlock = array();
			for ($nIndex = $nLowerLimit; $nIndex < $nUpperLimit; $nIndex++)
			{
				$arrayElement = ArrayGetValue($arrayList,$nIndex);
				if (is_array($arrayElement))
				{
					array_push($arrayListBlock,$arrayElement);
				}
			}
			$this->SetListArray($arrayListBlock);
			return true;	
		}
		
		function Join(
			$listJoin,
			$arrayJoinConditions,
			$arrayJoinFields
			)
		{
			$arrayList = $this->GetListArray();
			$arrayListJoin = $listJoin->GetListArray();
			
			foreach ($arrayList as &$arrayIterator)
			{
				$bMatched = false;
				foreach ($arrayListJoin as $arrayJoinIterator)
				{
					$bMatched = true;
					foreach ($arrayJoinConditions as $strJoinCondition)
					{
						if ($arrayIterator[$strJoinCondition] != $arrayJoinIterator[$strJoinCondition])
						{
							$bMatched = false;	
						}
					}
					if ($bMatched == false)
					{
						continue;	
					}
					
					foreach ($arrayJoinFields as $strField)
					{
						$arrayIterator[$strField] = $arrayJoinIterator[$strField];
					}
					
					break;
				}
				if ($bMatched == false)
				foreach ($arrayJoinFields as $strField)
				{
					if (ArrayGetValue($arrayIterator,$strField) == NULL)
					{
						$arrayIterator[$strField] = NULL;
					}
				}
				
					
			}
			
			$this->SetListArray($arrayList);
			return true;
		}
		
		
		function Aggregate(
			$arrayColumns,
			$strResultColumn)
		{
			if (ArrayCount($arrayColumns) == 0)
			{
				return false;	
			}
			if ($strResultColumn == '')
			{
				return false;	
			}
			
			$arrayList = $this->GetListArray();
			
			$arrayAggregatedList = array();	
			
			foreach ($arrayList as $arrayElement)
			{
				$strCombinedKey = '';
				foreach ($arrayElement as $strKey => $value)
				{
					if (ArrayValueExists($arrayColumns,$strKey))
					{
						$strValue = GetStringValue($value);
						$strCombinedKey .= ($strCombinedKey == '')?($strValue):(','.$strValue);
					}
				}

				foreach ($arrayElement as $strKey => $value)
				{
					if (ArrayValueExists($arrayColumns,$strKey))
					{
						$arrayAggregatedList[$strCombinedKey][$strKey] = $value;
					}
				}
				$arrayAggregatedList[$strCombinedKey][$strResultColumn] = 1 + intval(ArrayGetValue($arrayAggregatedList,$strCombinedKey,$strResultColumn));
			}
			$arrayAggregatedList = ArrayValues($arrayAggregatedList);
									
			$this->SetListArray($arrayAggregatedList);
			return true;
		}

	
		function CountValues()
		{
			$arrayCountedValues = array();
			$arrayList = $this->GetListArray();
			foreach ($arrayList as $arrayIterator)
			{
				foreach ($arrayIterator as $strKey => $strValue)
				{
					$arrayCountedValues["$strKey"]["$strValue"] = 1 + intval(ArrayGetValue($arrayCountedValues,"$strKey","$strValue"));
				}	
			}
			return $arrayCountedValues;
		}
		
		function GetStatistics()
		{
			$arrayStats = array();
			$arrayList = $this->GetListArray();
			foreach ($arrayList as $arrayItem)
			{
				foreach ($arrayItem as $strKey => $vValue)
				{
					if (is_numeric($vValue))
					{
						$arrayStats[$strKey]["count"] = 1 + intval(ArrayGetValue($arrayStats,$strKey,'count'));
						$arrayStats[$strKey]["sum"] = $vValue + intval(ArrayGetValue($arrayStats,$strKey,'sum'));
						$arrayStats[$strKey]["max"] = max(ArrayGetValue($arrayStats,$strKey,'max'),$vValue);
						if (ArrayKeyExists($arrayStats[$strKey],'min'))
						{
							$arrayStats[$strKey]['min'] = min(ArrayGetValue($arrayStats,$strKey,'min'),$vValue);		
						}
						else
						{
							$arrayStats[$strKey]["min"] = $vValue;
						}
					}
				}	
			}
			foreach ($arrayStats as $strKey => $arrayKeyStats)
			{
				$arrayStats[$strKey]["dif"] = $arrayStats[$strKey]["max"] - $arrayStats[$strKey]["min"];
				$arrayStats[$strKey]["avg"] = $arrayStats[$strKey]["sum"] / $arrayStats[$strKey]["count"];
			}
			return $arrayStats;
		}						
		
	}
		
	

	function GetFilterOptionsFromParams(
		$arrayParams,
		$arrayFilterConfig,
		$strPrefix = "",
		$nFilterParams = 10)
	{
		if ($strPrefix != "")
		{
			$strPrefix .= "_";
		}
		$arrayFilterOptions = array();
		for ($nIndex = 1; $nIndex <= $nFilterParams; $nIndex++)
		{
			$strFilterOption = ArrayGetValue($arrayParams,$strPrefix."filter$nIndex");
			if ($strFilterOption != "")
			{
				array_push($arrayFilterOptions,
					array(
						"field" => $arrayFilterConfig[$strFilterOption],
						"operator" => ArrayGetValue($arrayParams,$strPrefix."filter".$nIndex."_operator"),
						"value" => ArrayGetValue($arrayParams,$strPrefix."filter".$nIndex."_value")
					)
				);
			}
		}
		return $arrayFilterOptions;
	}

	function GetSortOptionsFromParams(
		$arrayParams,
		$arraySortConfig,
		$strPrefix = "",
		$nSortParams = 10)
	{
		if ($strPrefix != "")
		{
			$strPrefix .= "_";
		}
		$arraySortOptions = array();
		for ($nIndex = 1; $nIndex <= $nSortParams; $nIndex++)
		{
			$strSortOption = ArrayGetValue($arrayParams,$strPrefix."sort".$nIndex);
			$strSortOrder = ArrayGetValue($arrayParams,$strPrefix."sort".$nIndex."_order");
			if ($strSortOption != "")
			{
				if ($strSortOrder != "desc")
				{
					$strSortOrder = "asc";
				}
				$arraySortOptions[$arraySortConfig[$strSortOption]] = $strSortOrder;
			}
		}
		return $arraySortOptions;
	}

