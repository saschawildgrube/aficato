<?php



	function AlignListLevels($arrayList, $strLevelKey = 'level', $nRootLevel = 0, $nStartIndex = 0)
	{
		if (IsArray($arrayList) == false)
		{
			return [];	
		}
		if ($strLevelKey == '')
		{
			$strLevelKey = 'level';
		}
		
		$nLastCorrectLevel = 0;
		$nPreviousLevel = -1;
		$nLevelGap = 0;
		foreach ($arrayList as &$arrayItem)
		{
			$nLevel = ArrayGetValue($arrayItem,$strLevelKey);
			if ($nLevel <= $nPreviousLevel + 1)
			{
				$arrayItem[$strLevelKey] = $nLevel;
				$nLastCorrectLevel = $nLevel;
				$nLevelGap = 0;
			}
			else if ($nLevel > $nLastCorrectLevel + 1 && $nLevelGap > 0)
			{
				$nLevel -= $nLevelGap;
				$arrayItem[$strLevelKey] = $nLevel;
			}
			else
			{
				$nLevelGap = $nLevel - $nPreviousLevel - 1;
				$nLevel -= $nLevelGap;
				$arrayItem[$strLevelKey] = $nLevel;
			}
			$nPreviousLevel = $nLevel;
		}
		
		return $arrayList;	
	}



	function TransformListToTree($arrayList, $strLevelKey = 'level', $strChildrenKey = 'children', $nRootLevel = 0, $nStartIndex = 0)
	{
		if (IsArray($arrayList) == false)
		{
			return [];	
		}
		if ($strLevelKey == '')
		{
			$strLevelKey = 'level';
		}
		if ($strChildrenKey == '')
		{
			$strChildrenKey = 'children';
		}
		
		
		
		$arrayTree = [];
		$nLevel = $nRootLevel;
		for ($nIndex = $nStartIndex; $nIndex < ArrayCount($arrayList) && $nLevel >= $nRootLevel; $nIndex++)
		{
			$arrayItem = $arrayList[$nIndex];
			$nLevel = GetIntegerValue(ArrayGetValue($arrayItem,$strLevelKey));
			if ($nLevel == $nRootLevel)
			{
				$arrayChildren = TransformListToTree($arrayList,$strLevelKey,$strChildrenKey,$nLevel+1, $nIndex+1);
				if (ArrayCount($arrayChildren) > 0)
				{
					$arrayItem[$strChildrenKey] = $arrayChildren;
				}
				$arrayTree[] = $arrayItem;
			}
		}
		
		return $arrayTree;	
	
	}


