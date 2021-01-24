<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ParameterDefinition Float");
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
			$paramDef->AddParameterDefinitionFloat(
				"float1",
				"",
				"",
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Empty Strings");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionFloat(
				"float1",
				-0.5,
				23.0,
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,true,"");

			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionFloat(
				"float1",
				-0.5,
				23,
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"Non float value given as max");

		
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionFloat(
				"float1",
				10000000.0,
				-10000000.0,
				false);
			$this->TestCase_ParameterDefinition_IsValid($paramDef,false,"min is higher than max");

		

		}
		
		
		
	}
	
	

		
