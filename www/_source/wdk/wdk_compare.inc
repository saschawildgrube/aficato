<?php

	function CompareInteger($v1,$v2)
	{
		$n1 = intval($v1);	
		$n2 = intval($v2);
		if ($n1 < $n2)
		{
			return -1;	
		}
		if ($n1 > $n2)
		{
			return 1;	
		}
		return 0;
	}


	function CompareString($v1,$v2)
	{
		$str1 = "$v1";	
		$str2 = "$v2";
		$nCompare = strcasecmp($str1,$str2);
		if ($nCompare < 0)
		{
			return -1;	
		}
		if ($nCompare > 0)
		{
			return 1;	
		}
		return 0;
	}
	
	function CompareStringIgnoreHyphen($v1,$v2)
	{
		$v1_nohyphen = StringRemoveCharacters($v1,'-');
		$v2_nohyphen = StringRemoveCharacters($v2,'-');
		$nResult = CompareString($v1_nohyphen,$v2_nohyphen);
		if ($nResult != 0)
		{
			return $nResult;
		}
		$nLen1 = StringLength($v1);
		$nLen2 = StringLength($v2);
		if ($nLen1 < $nLen2)
		{
			return -1;	
		}
		if ($nLen1 > $nLen2)
		{
			return 1;
		}
		return 0;
	}

