<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		
		private $m_strWebservice;
		private $m_strItemID;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			$this->m_strWebservice = "demo/databasedemo";
			parent::__construct("DATABASEDEMO web service",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			return parent::CallbackInit();	
		}	
		
		function CallbackTest()
		{
			parent::CallbackTest();
			

			/*
			
			1. Add item
			2. Get item
			3. Set item
			4. Get item
			5. List
			6. Delete item
			7. Get item
			8. List item
			
			*/
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$strItemName1 = "test_initial";
			$strItemName2 = "test_reloaded";
			
			$strItemData1 = "This is a test!";
			$strItemData2 = u("The Data has changed! Now it contains \"umlaute\": ÄÖÜäöü");
			//$strItemData2 = "The Data has changed!"; 
			

			$this->Trace("1. ADD ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = $strItemName1;
			$arrayParams["item_data"] = $strItemData1; 
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strItemID = $consumer->GetResultValue("NEW_ITEM_ID");
			if ($this->m_strItemID == "")
			{
				$this->Trace("Error: item ID is empty");		
				return;
			}
			$this->Trace("New item ID: ".$this->m_strItemID);
			$this->Trace("");

	
		
		
			$this->Trace("2. GET ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $this->m_strItemID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strName = $consumer->GetResultValue("ITEM","ITEM_NAME");
			$strData = $consumer->GetResultValue("ITEM","ITEM_DATA");
			$this->Trace("ITEM_NAME = \"$strName\"");
			$this->Trace("ITEM_DATA = \"$strData\"");
			if ($strName != $strItemName1)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($strData != $strItemData1)
			{
				$this->Trace("Error: item data mismatch");		
				return;
			}
			$this->Trace("");
			
			

			$this->Trace("3. SET ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $this->m_strItemID;
			$arrayParams["item_name"] = $strItemName2; 
			$arrayParams["item_data"] = $strItemData2; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		


			$this->Trace("4. GET ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $this->m_strItemID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strName = $consumer->GetResultValue("ITEM","ITEM_NAME");
			$strData = $consumer->GetResultValueInsecure("ITEM","ITEM_DATA");

			
			$this->Trace("ITEM_NAME = \"$strName\"");
			$this->Trace("ITEM_DATA = \"$strData\"");
			if ($strName != $strItemName2)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($strData != $strItemData2)
			{
				$this->Trace("Error: item data mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("5. LIST");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["filter1"] = "item_name";
			$arrayParams["filter1_value"] = $strItemName2;
			$arrayParams["offset"] = "0";
			$arrayParams["blocksize"] = "10";
			$arrayParams["command"] = "list";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			
			if (ArrayCount($arrayList) != 1)
			{
				$this->Trace("Error: Exactly one item was expected!");		
				return;
			}
			if ($arrayList[0]["ITEM_ID"] != $this->m_strItemID)
			{
				$this->Trace("Error: item id mismatch");		
				return;
			}
			
			if ($arrayList[0]["ITEM_NAME"] != $strItemName2)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($arrayList[0]["ITEM_DATA"] != SecureOutput($strItemData2))
			{
				$this->Trace("Error: item data mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("6. DELETE ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $this->m_strItemID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");



			$this->Trace("7. GET ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $this->m_strItemID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Item should no longer exist!");		
				return;	
			}
			$this->Trace("");



			$this->Trace("8. LIST");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["filter1"] = "item_name";
			$arrayParams["filter1_value"] = $strItemName2;
			$arrayParams["offset"] = "0";
			$arrayParams["blocksize"] = "10";
			$arrayParams["command"] = "list";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			
			if (ArrayCount($arrayList) != 0)
			{
				$this->Trace("Error: No item was expected!");		
				return;
			}
			$this->Trace("");

			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			if ($this->m_strItemID != "")
			{
				$this->Trace("DELETE ITEM");
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["item_id"] = $this->m_strItemID;
				$arrayParams["command"] = "delete";
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace($consumer->GetServiceOutput());
				$this->Trace("");
			}
			
			return true;
		}
		
		
	}
	
	

		
