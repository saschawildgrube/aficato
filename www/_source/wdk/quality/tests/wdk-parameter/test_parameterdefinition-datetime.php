<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ParameterDefinition DateTime");
		}
		
		function OnInit()
		{
			parent::OnInit();
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

		
		function OnTest()
		{
			parent::OnTest();
			
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"datetime1",
				"",
				"",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,true,"");
		

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"datetime1",
				"2008-12-24 10:00:00",
				"2009-01-02 00:00:00",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,true,"");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"date1",
				"200812-24 12:00:00",
				"2009-01-02 00:00:0",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Wrong date pattern");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"date1",
				"2008-12-24 12:00:00",
				"2009-15-02 24:60:60",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Invalid date");
			
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"date1",
				"2008-12-30 00:00:00",
				"2008-01-01 00:00:00",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Max date before min date");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDateTime(
				"date1",
				"2008-01-01 23:00:00",
				"2008-01-01 01:00:00",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Max date before min date");


		}
		
		
		
	}
	
	

		
