<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		
		private $m_strWebservice;
		private $m_arrayItemIDs;
		
		function __construct()
		{
			$this->m_strWebservice = "demo/databasedemo";
			$this->m_arrayItemIDs = array();

			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			
			parent::__construct("DATABASEDEMO web service",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			return parent::OnInit();	
		}	
		
		function OnTest()
		{
			parent::OnTest();
			
			
			
			



			/*
			
			1. Add item 1
			2. Add item 2
			3. Add item 3
			
			4. List all
			5. List in

			Cleanup. Delete all

			*/
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$strItemName1 = "test_1";
			$strItemName2 = "test_2";
			$strItemName3 = "test_3";
			
			$strItemData1 = "This is a test!";
			$strItemData2 = "Another test";
			$strItemData3 = "Yet another test";
			

			$this->Trace("1. ADD ITEM 1");
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
			$strNewItemID = $consumer->GetResultValue("NEW_ITEM_ID");
			if ($strNewItemID == "")
			{
				$this->Trace("Error: item ID is empty");		
				return;
			}
			$this->Trace("New item ID: ".$strNewItemID);
			$this->Trace("");
			
			$this->m_arrayItemIDs[] = $strNewItemID;			





			$this->Trace("2. ADD ITEM 2");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = $strItemName2;
			$arrayParams["item_data"] = $strItemData2; 
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strNewItemID = $consumer->GetResultValue("NEW_ITEM_ID");
			if ($strNewItemID == "")
			{
				$this->Trace("Error: item ID is empty");		
				return;
			}
			$this->Trace("New item ID: ".$strNewItemID);
			$this->Trace("");
			
			$this->m_arrayItemIDs[] = $strNewItemID;			



	
	
	
			$this->Trace("3. ADD ITEM 3");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = $strItemName3;
			$arrayParams["item_data"] = $strItemData3; 
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strNewItemID = $consumer->GetResultValue("NEW_ITEM_ID");
			if ($strNewItemID == "")
			{
				$this->Trace("Error: item ID is empty");		
				return;
			}
			$this->Trace("New item ID: ".$strNewItemID);
			$this->Trace("");
			
			$this->m_arrayItemIDs[] = $strNewItemID;			





			$this->Trace("4. LIST ALL");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["sort1"] = "item_id";
			$arrayParams["sort1_order"] = "asc";

/*			$arrayParams["filter1"] = "item_name";
			$arrayParams["filter1_value"] = $strItemName2;*/
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
			
			if (ArrayCount($arrayList) != 3)
			{
				$this->Trace("Error: Exactly three item were expected!");		
				return;
			}
			if ($arrayList[0]["ITEM_ID"] != $this->m_arrayItemIDs[0])
			{
				$this->Trace("Error: item 1 id mismatch");		
				return;
			}
			
			if ($arrayList[0]["ITEM_NAME"] != $strItemName1)
			{
				$this->Trace("Error: item 1 name mismatch");		
				return;
			}
			if ($arrayList[0]["ITEM_DATA"] != SecureOutput($strItemData1))
			{
				$this->Trace("Error: item 1 data mismatch");		
				return;
			}
			$this->Trace("");






			$this->Trace("8. LIST IN");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["filter1"] = "item_name";
			$arrayParams["filter1_operator"] = "in";
			$arrayParams["filter1_value"] = $strItemName1.",".$strItemName2;
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
			
			if (ArrayCount($arrayList) != 2)
			{
				$this->Trace("Error: Exactly two item were expected!");		
				return;
			}
			if ($arrayList[0]["ITEM_ID"] != $this->m_arrayItemIDs[0])
			{
				$this->Trace("Error: item 1 id mismatch");		
				return;
			}
			if ($arrayList[1]["ITEM_ID"] != $this->m_arrayItemIDs[1])
			{
				$this->Trace("Error: item 2 id mismatch");		
				return;
			}
			
			$this->Trace("");
			$this->Trace("");

			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
			foreach ($this->m_arrayItemIDs as $strItemID)
			{
				$this->Trace("DELETE ITEM $strItemID");
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["item_id"] = $strItemID;
				$arrayParams["command"] = "delete";
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace($consumer->GetServiceOutput());
				$this->Trace("");
			}

			return true;
		}
		
		
	}
	
	

		
