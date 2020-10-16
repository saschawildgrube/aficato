<?php
	
	require_once(GetWDKDir()."wdk_routing.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeRoutingAlias");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_MakeRoutingAlias($strString,$bMakeLowerCase,$strExpectedResult)
		{
			$this->Trace("TestCase_MakeRoutingAlias");
			$this->Trace("strString               : \"$strString\"");
			$this->Trace("bMakeLowerCase          : ".RenderBool($bMakeLowerCase));
			
			$this->Trace("Expected Result         : \"$strExpectedResult\"");
			$strResult = MakeRoutingAlias($strString,$bMakeLowerCase);
			$this->Trace("MakeRoutingAlias returns: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$this->TestCase_MakeRoutingAlias(
				"start",
				false,
				"start");

			$this->TestCase_MakeRoutingAlias(
				u("L'État c'est moi"),
				false,
				"L-Etat-c-est-moi");

			$this->TestCase_MakeRoutingAlias(
				u("Übermut kommt vor dem Fall"),
				true,
				"uebermut-kommt-vor-dem-fall");

	
		}
		

	}
	
	

		
