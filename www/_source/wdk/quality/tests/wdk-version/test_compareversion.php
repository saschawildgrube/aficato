<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CompareVersion");
		}
		

		function TestCase_CompareVersion(
			$v1,
			$v2,
			$nExpected)
		{ 
			$this->Trace("TestCase_CompareVersion");
	 
			$this->Trace("version 1: ".RenderValue($v1));
			$this->Trace("version 2: ".RenderValue($v2));

			$this->Trace("Expected: $nExpected");

			$nResult = CompareVersion($v1,$v2);
			$this->Trace("CompareVersion returns: $nResult");
			
			if ($nResult != $nExpected)
			{
				$this->Trace("Expected result does not match.");	
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
				$this->Trace("");
				$this->Trace("");
				return;
			}
			
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$v1 = array(
				"MAJOR" => 0);
			$v2 = array(
				"MAJOR" => 1);
			$this->TestCase_CompareVersion($v1,$v2,-1);

			$v1 = array(
				"MAJOR" => 1);
			$v2 = array(
				"MAJOR" => 1);
			$this->TestCase_CompareVersion($v1,$v2,0);

			$v1 = array(
				"MAJOR" => 1);
			$v2 = array(
				"MAJOR" => 0);
			$this->TestCase_CompareVersion($v1,$v2,1);



			$v1 = array(
				"MAJOR" => 1,
				"MINOR" => 0,
				"PATCH" => 23);
			$v2 = array(
				"MAJOR" => 1,
				"MINOR" => 1, 
				"PATCH" => 54);
			$this->TestCase_CompareVersion($v1,$v2,-1);

			$v1 = array(
				"MAJOR" => 1,
				"MINOR" => 1,
				"PATCH" => 23);
			$v2 = array(
				"MAJOR" => 1,
				"MINOR" => 1, 
				"PATCH" => 23);
			$this->TestCase_CompareVersion($v1,$v2,0);

			$v1 = array(
				"MAJOR" => 1,
				"MINOR" => 1,
				"PATCH" => 23);
			$v2 = array(
				"MAJOR" => 1,
				"MINOR" => 0, 
				"PATCH" => 54);
			$this->TestCase_CompareVersion($v1,$v2,1);



			$v1 = array(
				"MAJOR" => 1,
				"PATCH" => 23);
			$v2 = array(
				"MAJOR" => 1, 
				"PATCH" => 24);
			$this->TestCase_CompareVersion($v1,$v2,-1);

			$v1 = array(
				"MAJOR" => 1,
				"PATCH" => 24);
			$v2 = array(
				"MAJOR" => 1,
				"PATCH" => 24);
			$this->TestCase_CompareVersion($v1,$v2,0);

			$v1 = array(
				"MAJOR" => 1,
				"PATCH" => 25);
			$v2 = array(
				"MAJOR" => 1,
				"PATCH" => 24);
			$this->TestCase_CompareVersion($v1,$v2,1);
			

			$v1 = "2.0";
			$v2 = "2.1";
			$this->TestCase_CompareVersion($v1,$v2,-1);

			$v1 = "2.1";
			$v2 = "2.1";
			$this->TestCase_CompareVersion($v1,$v2,0);

			$v1 = "2.2";
			$v2 = "2.1";
			$this->TestCase_CompareVersion($v1,$v2,1);



			$v1 = "2.0-5ubuntu1.3";
			$v2 = "2.0-5ubuntu1.4";
			$this->TestCase_CompareVersion($v1,$v2,-1);

			$v1 = "2.0-5ubuntu1.4";
			$v2 = "2.0-5ubuntu1.4";
			$this->TestCase_CompareVersion($v1,$v2,0);

			$v1 = "2.0-5ubuntu1.4";
			$v2 = "2.0-5ubuntu1.3";
			$this->TestCase_CompareVersion($v1,$v2,1);



		}
		
		
	}
	
	


		
