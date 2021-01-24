<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ParseVersion");
		}
		

		function TestCase_ParseVersion(
			$strParam,
			$nComponents,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseVersion");
	 
			$this->Trace("String     = \"$strParam\"");
			$this->Trace("Components = ".intval($nComponents));

			$this->Trace("Expected:");
			$this->Trace($arrayExpectedResult);


			$arrayResult = ParseVersion($strParam,$nComponents);
			$this->Trace("ParseVersion returns:");
			$this->Trace($arrayResult);
			
			if ($arrayResult != $arrayExpectedResult)
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


			$arrayExpectedResult = array(
				"VERSION" => "0",
				"MAJOR" => 0);
			$this->TestCase_ParseVersion("",1,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "23",
				"MAJOR" => 23);
			$this->TestCase_ParseVersion(23,1,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1",
				"MAJOR" => 1);
			$this->TestCase_ParseVersion("1",1,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1",
				"MAJOR" => 1);
			$this->TestCase_ParseVersion("1.2",1,$arrayExpectedResult);






			$arrayExpectedResult = array(
				"VERSION" => "0.0.0",
				"MAJOR" => 0,
				"MINOR" => 0,
				"PATCH" => 0);
			$this->TestCase_ParseVersion("",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "23.0.0",
				"MAJOR" => 23,
				"MINOR" => 0,
				"PATCH" => 0);
			$this->TestCase_ParseVersion(23,3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.0.0",
				"MAJOR" => 1,
				"MINOR" => 0,
				"PATCH" => 0);
			$this->TestCase_ParseVersion("1",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.0",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 0);
			$this->TestCase_ParseVersion("1.2",3,$arrayExpectedResult);
			
			$arrayExpectedResult = array(
				"VERSION" => "1.2.3",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3);
			$this->TestCase_ParseVersion("1.2.3",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.3",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3);
			$this->TestCase_ParseVersion("1.2.3.4",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.3",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3);
			$this->TestCase_ParseVersion("1.2.3.4.5",3,$arrayExpectedResult);







			$arrayExpectedResult = array(
				"VERSION" => "0.0.0.0",
				"MAJOR" => 0,
				"MINOR" => 0,
				"PATCH" => 0,
				"BUILD" => 0);
			$this->TestCase_ParseVersion("",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "23.0.0.0",
				"MAJOR" => 23,
				"MINOR" => 0,
				"PATCH" => 0,
				"BUILD" => 0);
			$this->TestCase_ParseVersion(23,4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.0.0.0",
				"MAJOR" => 1,
				"MINOR" => 0,
				"PATCH" => 0,
				"BUILD" => 0);
			$this->TestCase_ParseVersion("1",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.0.0",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 0,
				"BUILD" => 0);
			$this->TestCase_ParseVersion("1.2",4,$arrayExpectedResult);
			
			$arrayExpectedResult = array(
				"VERSION" => "1.2.3.0",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3,
				"BUILD" => 0);
			$this->TestCase_ParseVersion("1.2.3",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.3.4",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3,
				"BUILD" => 4);
			$this->TestCase_ParseVersion("1.2.3.4",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.2.3.4",
				"MAJOR" => 1,
				"MINOR" => 2,
				"PATCH" => 3,
				"BUILD" => 4);
			$this->TestCase_ParseVersion("1.2.3.4.5",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.13.4",
				"MAJOR" => 1,
				"MINOR" => 13,
				"PATCH" => 4,
				"PACKAGE" => "2ubuntu1.2");
			$this->TestCase_ParseVersion("1.13.4-2ubuntu1.2",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.13.4.0",
				"MAJOR" => 1,
				"MINOR" => 13,
				"PATCH" => 4,
				"BUILD" => 0,
				"PACKAGE" => "2ubuntu1.2");
			$this->TestCase_ParseVersion("1.13.4-2ubuntu1.2",4,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.13.4",
				"MAJOR" => 1,
				"MINOR" => 13,
				"PATCH" => 4);
			$this->TestCase_ParseVersion("1.13.4-",3,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"VERSION" => "1.13.4.0",
				"MAJOR" => 1,
				"MINOR" => 13,
				"PATCH" => 4,
				"BUILD" => 0);
			$this->TestCase_ParseVersion("1.13.4-",4,$arrayExpectedResult);
		



		}
		
		
	}
	
	


		
