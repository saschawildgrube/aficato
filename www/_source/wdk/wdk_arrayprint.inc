<?php

	function ArrayPrint($array1)
	{
		print_r($array1);
	}

	function ArrayRender($array1)
	{
		return print_r($array1,true);
	}

	function ArrayRenderList($array1)
	{
		if (!is_array($array1))
		{
			return "";	
		}
		$strList = "";
		$nCount = ArrayCount($array1);
		$nIndex = 0;
		foreach ($array1 as $vValue)
		{
			if ($nIndex > 0)
			{
				$strList .= ", ";	
			}
			if (is_array($vValue))
			{
				$strList .= "array";	
			}
			else
			{
				$strList .= "\"".$vValue."\"";	
			}
			$nIndex++;
		}
		return $strList;
	}

