<?php
	
	require_once(GetWDKDir()."wdk_entity_session.inc");
	require_once(GetWDKDir()."wdk_entity_log.inc");
	require_once(GetWDKDir()."wdk_entity_job.inc");
	require_once(GetWDKDir()."wdk_entity_user.inc");
	require_once(GetWDKDir()."wdk_entity_entitlement.inc");
	require_once(GetWDKDir()."wdk_entity_dataitem.inc");
	require_once(GetWDKDir()."wdk_entity_test.inc");
		
	require_once(GetWDKDir()."wdk_entity_article.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Definition of all WDK supported Entities");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}


		function TestCase_CheckEntity($strEntity,$arrayEntityDefinition)
		{
			$bTestcase = true;
			$this->Trace("TestCase_CheckEntity for $strEntity");
			
			//$this->Trace("Entity Definition:");
			//$this->Trace($arrayEntityDefinition);
	
			
			
			$entityDef = new CEntityDefinitions();
			$entityDef->SetEntityDefinitions(array($arrayEntityDefinition));
			
			$arrayErrors = array();
			$bResult = $entityDef->CheckEntityDefinitions($arrayErrors,ENTITYDEF_MODULE);
			$this->Trace("Result for ENTITYDEF_MODULE: ".RenderBool($bResult));
			if ($bResult != true)
			{
				$this->Trace($arrayErrors);
				$bTestcase = false;
			}
			
			$arrayErrors = array();
			$bResult = $entityDef->CheckEntityDefinitions($arrayErrors,ENTITYDEF_WEBSERVICE);
			$this->Trace("Result for ENTITYDEF_WEBSERVICE: ".RenderBool($bResult));
			if ($bResult != true)
			{
				$this->Trace($arrayErrors);
				$bTestcase = false;
			}
			
			if ($bTestcase == true)			
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
			$this->TestCase_CheckEntity("session",GetEntityDefinitionSession());
			$this->TestCase_CheckEntity("log",GetEntityDefinitionLog());
			$this->TestCase_CheckEntity("user",GetEntityDefinitionUser());
			$this->TestCase_CheckEntity("entitlement",GetEntityDefinitionEntitlement());
			$this->TestCase_CheckEntity("job",GetEntityDefinitionJob());
			$this->TestCase_CheckEntity("dataitem",GetEntityDefinitionDataitem());
			$this->TestCase_CheckEntity("test",GetEntityDefinitionTest());
			
			$this->TestCase_CheckEntity("article",GetEntityDefinitionArticle());
		}
		
		
		
	}
	
	

		
