<?php



	function AlignListLevels($arrayList, $strLevelKey = 'level', $nRootLevel = 0, $nStartIndex = 0)
	{
		if (IsArray($arrayList) == false)
		{
			return [];	
		}
		if (ArrayCount($arrayList) == 0)
		{
			return [];	
		}
		if ($strLevelKey == '')
		{
			$strLevelKey = 'level';
		}

		// Align the items on the lowest level
		$bContinue = true;
		$nBaseLevel = 0;
		while ($bContinue)
		{
			$nLowestLevel = INTEGER_MAX;
			foreach ($arrayList as $arrayItem)
			{
				$nLowestLevel = min($nLowestLevel,max($nBaseLevel,ArrayGetValue($arrayItem,$strLevelKey)));
			}
			$bContinue = false;
			foreach ($arrayList as &$arrayItem2)
			{
				$nLevel = ArrayGetValue($arrayItem2,$strLevelKey);
				if ($nLevel >= $nBaseLevel)
				{
					if ($nLevel == $nLowestLevel)
					{
						$bContinue = true;
						$arrayItem2[$strLevelKey] = $nBaseLevel;
					}
				}
			}
			$nBaseLevel++;
		}
		
		$nLastCorrectLevel = 0;
		$nPreviousLevel = -1;
		$nLevelGap = 0;
		foreach ($arrayList as &$arrayItem3)
		{
			$nLevel = ArrayGetValue($arrayItem3,$strLevelKey);
			if ($nLevel <= $nPreviousLevel + 1)
			{
				$arrayItem3[$strLevelKey] = $nLevel;
				$nLastCorrectLevel = $nLevel;
				$nLevelGap = 0;
			}
			else if ($nLevel > $nLastCorrectLevel + 1 && $nLevelGap > 0)
			{
				$nLevel -= $nLevelGap;
				$arrayItem3[$strLevelKey] = $nLevel;
			}
			else
			{
				$nLevelGap = $nLevel - $nPreviousLevel - 1;
				$nLevel -= $nLevelGap;
				$arrayItem3[$strLevelKey] = $nLevel;
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


