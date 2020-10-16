<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CheckParameter");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}


		function TestCase_CheckParameters($paramDef,$arrayParams,$arrayExpectedErrors)
		{
			$this->Trace("TestCase_CheckParameters");
			
			$this->Trace("Param Definition:");
			$this->Trace($paramDef->GetParamDefData());
			$this->Trace("Params:");
			$this->Trace($arrayParams);
			$this->Trace("Expected Result:");
			$this->Trace($arrayExpectedErrors);
	
			$arrayErrors = array();
			$paramDef->CheckParameters($arrayParams,$arrayErrors,false);
			
			$this->Trace("Result:");
			$this->Trace($arrayErrors);
	
				
			if ($arrayErrors == $arrayExpectedErrors)
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
			$paramDef->AddParameterDefinition_userid();
			$paramDef->AddParameterDefinition_username();
			$paramDef->AddParameterDefinition_trace();
			$arrayParams = array();
			$arrayParams["user_name"] = "abc";
			$arrayParams["user_id"] = u("הצü");
			$arrayExpectedErrors = array();
			$arrayExpectedErrors["user_id"] = "PARAMETER_USER_ID_INVALID_CHARACTERS";
			$this->TestCase_CheckParameters($paramDef,$arrayParams,$arrayExpectedErrors);
		



			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionDate("date1","","2010-01-01");
			$paramDef->AddParameterDefinitionDate("date2","2001-09-11","");
			$paramDef->AddParameterDefinitionDateTime("datetime1","","2010-01-01 12:30:25");
			$paramDef->AddParameterDefinitionDateTime("datetime2","2001-09-11 09:35:11","");
			$paramDef->AddParameterDefinition_trace();
			$arrayParams = array();
			$arrayParams["date1"] = "2010-01-02";
			$arrayParams["date2"] = "2001-09-10";
			$arrayParams["datetime1"] = "2010-01-01 12:30:26";
			$arrayParams["datetime2"] = "2000-01-01 00:00:00";
			$arrayExpectedErrors = array();
			$arrayExpectedErrors["date1"] = "PARAMETER_DATE1_TOOLATE";
			$arrayExpectedErrors["date2"] = "PARAMETER_DATE2_TOOEARLY";
			$arrayExpectedErrors["datetime1"] = "PARAMETER_DATETIME1_TOOLATE";
			$arrayExpectedErrors["datetime2"] = "PARAMETER_DATETIME2_TOOEARLY";
			$this->TestCase_CheckParameters($paramDef,$arrayParams,$arrayExpectedErrors);


			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionFloat("float1",0.0,1000.0);
			$paramDef->AddParameterDefinitionFloat("float2",-0.5,23.0);
			$paramDef->AddParameterDefinitionFloat("float3",-0.5,23.0);
			$paramDef->AddParameterDefinitionFloat("float4",-1.5,23.0);
			$paramDef->AddParameterDefinition_trace();
			$arrayParams = array();
			$arrayParams["float1"] = "1000.5";
			$arrayParams["float2"] = "-3-3";
			$arrayParams["float3"] = "bogus";
			$arrayParams["float4"] = "-3.0";
			$arrayExpectedErrors = array();
			$arrayExpectedErrors["float1"] = "PARAMETER_FLOAT1_MAX";
			$arrayExpectedErrors["float2"] = "PARAMETER_FLOAT2_INVALID";  
			$arrayExpectedErrors["float3"] = "PARAMETER_FLOAT3_INVALID_CHARACTERS";  
			$arrayExpectedErrors["float4"] = "PARAMETER_FLOAT4_MIN";
			$this->TestCase_CheckParameters($paramDef,$arrayParams,$arrayExpectedErrors);


			
			
		}
		
		
		
	}
	
	

		
