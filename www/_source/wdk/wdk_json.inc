<?php


	function JsonEncode($arrayData)
	{
		if (!is_array($arrayData))
		{
			return false;	
		}
		return json_encode($arrayData,JSON_PRETTY_PRINT);
	}

	function JsonDecode($strJson)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$arrayData = json_decode($strJson,true);
		$ea->End();
		if (!is_array($arrayData))
		{
			return false;	
		}
		return $arrayData;
	}
	
