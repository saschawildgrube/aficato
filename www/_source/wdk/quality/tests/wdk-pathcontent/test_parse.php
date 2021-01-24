<?php
		
	require_once(GetWDKDir()."wdk_pathcontent.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ParsePathContent()");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_ParsePathContent(
			$strData,
			$arrayExpectedData)
		{ 
			$bTestcase = true;
			$this->Trace("TestCase: ParsePathContent() ");
			
			$this->Trace("Source Data:");
			$this->Trace($strData);
			$this->Trace("");
			
			$this->Trace("Expected Data Array:");
			$this->Trace($arrayExpectedData);
			$this->Trace("");
			
			$bCodeInjection = false;
			$arrayData = ParsePathContent($strData);
			
			$this->Trace("Result Data");
			$this->Trace($arrayData);
		
			if ($arrayData != $arrayExpectedData)
			{
				$this->Trace("Testcase FAILED because of unexpected result of ParsePathContent()");
				$bTestcase = false;
			}
			
			if ($bCodeInjection == true)
			{
				$this->Trace("Testcase FAILED because of code injection vulnerability in ParsePathCOntent()");
				$bTestcase = false;
				
			}
			
			if ($bTestcase == true)
			{
				$this->Trace("Testcase PASSED!");
			} 
			$this->Trace("");
			$this->Trace("");
		}
		
		
		function OnTest()
		{
			parent::OnTest();
			
					
		
			$strData = "";
			$arrayData = array();
			$this->TestCase_ParsePathContent($strData,$arrayData);
			

			$strData =
"#0,0
#1,1
#2,2
#3,3
#4,4";
			$arrayData = array();
			for ($nIndex = 0; $nIndex < 5; $nIndex++)
			{
				array_push($arrayData,$nIndex);
			}
			$this->TestCase_ParsePathContent($strData,$arrayData);


		
			$strData =
"apple,red
banana,yellow
salad,green
orange,orange";
			$arrayData = array();
			$arrayData["apple"] = "red";
			$arrayData["banana"] = "yellow";
			$arrayData["salad"] = "green";
			$arrayData["orange"] = "orange";
			$this->TestCase_ParsePathContent($strData,$arrayData);

		 
		
			$strData =
"apple,red
banana\"] = \"yellow\"; \$bCodeInjection = true; \$arrayData[\"banana,yellow
salad,green
orange,orange";
			$arrayData = array();
			$arrayData["apple"] = "red";
			$arrayData["banana\"] = \"yellow\"; \$bCodeInjection = true; \$arrayData[\"banana"] = "yellow";
			$arrayData["salad"] = "green";
			$arrayData["orange"] = "orange";
			$this->TestCase_ParsePathContent($strData,$arrayData);   
		
		
		}
		

		
	}
	
	

		
