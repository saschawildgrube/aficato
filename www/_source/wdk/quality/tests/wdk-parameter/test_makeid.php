<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test MakeID");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}

		function TestCase_MakeID($vRaw,$strExpectedResult)
		{
			$this->Trace("TestCase_MakeID");
			$this->Trace("vRaw = ".RenderValue($vRaw));
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = MakeID($vRaw);
			$this->Trace("MakeID returns: \"$strResult\"");
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
			
			$this->TestCase_MakeID("abcd","abcd");
			$this->TestCase_MakeID("Ürmel","rmel");
			$this->TestCase_MakeID("Two Words","twowords");
			$this->TestCase_MakeID("mail@domain.com","maildomaincom");
			$this->TestCase_MakeID("","");
			$this->TestCase_MakeID(array(),"");
			$this->TestCase_MakeID(array("a" => "alpha", "b" => "beta"),"aalphabbeta");
			$this->TestCase_MakeID(array("b" => "beta", "a" => "alpha"),"aalphabbeta");
			$this->TestCase_MakeID(array("beta", "alpha"),"0beta1alpha"); 
			
			
		}
		
	}
	
	

		
