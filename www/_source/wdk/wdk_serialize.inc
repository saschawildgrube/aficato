<?php

	function ObjectSerialize($vObject)
	{
		return serialize($vObject);
	}

	function ObjectUnserialize($strSerialized)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$vObject = unserialize($strSerialized);
		$ea->End();
		return $vObject;
	}
	