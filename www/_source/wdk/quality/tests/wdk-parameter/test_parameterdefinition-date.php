<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ParameterDefinition Date");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}


		function TestCase_ParameterDefinition_IsValid($paramDef,$bExpected,$strInfo)
		{
			$this->Trace("TestCase_ParameterDefinition_IsValid");
			
			$this->Trace("Param Definition:");
			$this->Trace($paramDef->GetParamDefData());
			$this->Trace("Expected Result: ".RenderBool($bExpected));
			$this->Trace($strInfo);
				
			$bResult = $paramDef->IsValid();
			
			$this->Trace("Result: ".RenderBool($bResult));
			
			if ($bResult == $bExpected)
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
			
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate(
				"date1",
				"",
				"",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,true,"");
		

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate(
				"date1",
				"2008-12-24",
				"2009-01-02",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,true,"");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate(
				"date1",
				"200812-24",
				"2009-01-02",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Wrong date pattern");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate(
				"date1",
				"2008-12-24",
				"2009-15-02",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Invalid date");
			
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate(
				"date1",
				"2008-12-30",
				"2008-01-01",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Max date before min date");


		}
		
		
		
	}
	
	

		
