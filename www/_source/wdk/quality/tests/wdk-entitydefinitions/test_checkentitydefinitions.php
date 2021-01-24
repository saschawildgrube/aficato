<?php
	
	require_once(GetWDKDir()."wdk_entitydefinitions.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CheckEntityDefinitions");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}


		function TestCase_CheckEntityDefinitions($arrayEntityDefinitions,$nCheckMode,$bExpectedResult,$arrayExpectedErrors)
		{
			$this->Trace("TestCase_CheckEntityDefinitions");
			
			$this->Trace("Entity Definition Data:");
			$this->Trace($arrayEntityDefinitions);
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult));
			$this->Trace("Expected Error:");
			$this->Trace($arrayExpectedErrors);
	
			$arrayErrors = array();
			
			$entityDef = new CEntityDefinitions();
			$entityDef->SetEntityDefinitions($arrayEntityDefinitions);
			
			$bResult = $entityDef->CheckEntityDefinitions($arrayErrors,$nCheckMode);
			$this->Trace("Result: ".RenderBool($bResult));
			$this->Trace($arrayErrors);
	
				
			if ($bResult == $bExpectedResult)
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

			$arrayEntityDefinitions = array(
				"demoitem" => array(
					"indexattribute" => "id",
					"default" => true,
					"indexstate" => "view",
					"userfriendlyattribute" => "name", 
					"blocksize" => 10,
					"noitemsfoundmessage" => "?TID_DEMOITEMMANAGER_INFO_DEMOITEM_NOITEMSFOUND?",
					"dbtablename" => "System-DemoItem",
					"uniqueindices" => array(
						"unique" => array("name","floatvalue")
						)
					,
					"webservice" => array(
						"name" => "demo/databasedemo",
						"itemtag" => "ITEM",
						"newitemidtag" => "NEWITEMID"
						/*,
						"commands" => array(
							"list" => array(
								"name" => "demoitemlist")
							)
							*/
						)
					,
					"sorting" => array(
						"defaultattribute" => "name",
						"defaultorder" => "asc")
					,
					"filteroptions" => array(
						"submitbuttontext" => "?TID_DEMOITEMMANAGER_BUTTON_FILTER?",
						"reset" => 1,
						"reseticon" => "undo",
						"resettooltip" => "?TID_DEMOITEMMANAGER_TOOLTIP_RESET?",
						"formstyle" => "horizontal",
						"formid" => "",
						"filters" => array(
							"searchbyname" => array(
								"webservicefilteroption" => "name",
								"label" => "?TID_DEMOITEMMANAGER_LABEL_SEARCH?",
								"type" => "search")
							)
						)
					,
					"states" => array(
						"add",
						"view",
						"modify",
						"delete")
					,
					"tasks" => array(
						"view" => array(
							"state" => "view",
							"icon" => "view",
							"tooltip" => "?TID_DEMOITEMMANAGER_TOOLTIP_VIEW?")
						,
						"modify" => array(
							"state" => "modify",
							"icon" => "modify",
							"tooltip" => "?TID_DEMOITEMMANAGER_TOOLTIP_MODIFY?")
						,
						"delete" => array(
							"state" => "delete",
							"icon" => "delete",
							"tooltip" => "?TID_DEMOITEMMANAGER_TOOLTIP_DELETE?")
						)
					,
					"tabs" => array(
						"tab1" => "?TID_DEMOITEMMANAGER_TABLEHEADER_CREATION?",
						"tab2" => "?TID_DEMOITEMMANAGER_TABLEHEADER_LASTCHANGE?"
						)
					,
					"attributes" => array( 
						"id" => array(
							"webserviceparam" => true,
							"sortoption" => true,
							//"sharecolumn" => true,
							"type" => "numericid",
							//"tableheader" => "?TID_DEMOITEMMANAGER_TABLEHEADER_ID?",
							//"formlabel" => "?TID_DEMOITEMMANAGER_LABEL_ID?",
							"readonly" => true,
							"hideinlist" => true,
							"hideinview" => true,
							"indexlink" => false)
						,
						"name" => array(
							"webserviceparam" => true,
							"sortoption" => true,
							"filteroption" => true,
							"type" => "string",
							"maxlen" => 255,
							"tableheader" => "?TID_DEMOITEMMANAGER_TABLEHEADER_NAME?",
							"formlabel" => "?TID_DEMOITEMMANAGER_LABEL_NAME?",
							"indexlink" => true)
						,
						"data" => array(
							"webserviceparam" => true,
							"type" => "string",
							"control" => "textarea",
							"maxlen" => 2000,
							"tableheader" => "",
							"formlabel" => "?TID_DEMOITEMMANAGER_LABEL_DATA?",
							"hideinlist" => true,
							"hideinview" => false,
							"indexlink" => false)
						,
						"floatvalue" => array(
							"webserviceparam" => true,
							"type" => "float",
							"max" => 2000.45,
							"min" => -256.78901,
							"floatprecision" => 10,
							"tableheader" => "",
							"formlabel" => "?TID_DEMOITEMMANAGER_LABEL_FLOAT_VALUE?",
							"hideinview" => false,
							"indexlink" => false)
						,
						"creation" => array(
							"webserviceparam" => true,
							"sortoption" => true,
							"type" => "datetime",
							"tableheader" => "?TID_DEMOITEMMANAGER_TABLEHEADER_CREATION?",
							"formlabel" => "",
							"readonly" => true,
							"tab" => "tab1")	
						,
						"lastchange" => array(
							"webserviceparam" => true,
							"sortoption" => true,
							"type" => "datetime",
							"tableheader" => "?TID_DEMOITEMMANAGER_TABLEHEADER_LASTCHANGE?",
							"formlabel" => "",
							"readonly" => true,
							"tab" => "tab2")
					)		
				)
			);

			
			$arrayExpectedErrors = array();
			//$arrayExpectedErrors["userid"] = "PARAMETER_USERID_INVALID_CHARACTERS";
			
			$this->TestCase_CheckEntityDefinitions(
				$arrayEntityDefinitions,
				ENTITYDEF_MODULE,
				true,
				$arrayExpectedErrors);

			$this->TestCase_CheckEntityDefinitions(
				$arrayEntityDefinitions,
				ENTITYDEF_WEBSERVICE,
				true,
				$arrayExpectedErrors);
		
			
			
		}
		
		
		
	}
	
	

		
