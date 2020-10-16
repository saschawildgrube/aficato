<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strItemID;
		
		function __construct()
		{
			$this->m_strWebservice = "$$$ws1$$$/$$$ws2$$$";
			$this->m_strItemID = "";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service $$$ws1$$$/$$$ws2$$$",$arrayConfig);
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
			
			$strName1 = "test initial";
			$strName2 = "test reloaded";
			
			$strContent1 = "This is a test!";
			$strContent2 = u("The Data has changed! Now it contains \"umlaute\": ÄÖÜäöü");


			$this->Trace("1. ADD ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["content"] = $strContent1; 
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strItemID = $consumer->GetResultValue("NEW_$$$ENTITYNAME$$$_ID");
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
			$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strName = $consumer->GetResultValue("$$$ENTITYNAME$$$","NAME");
			$strContent = $consumer->GetResultValue("$$$ENTITYNAME$$$","CONTENT");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("CONTENT = \"$strContent\"");
			if ($strName != $strName1)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($strContent != $strContent1)
			{
				$this->Trace("Error: item content mismatch");		
				return;
			}
			$this->Trace("");
			
			

			$this->Trace("3. SET ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
			$arrayParams["name"] = $strName2; 
			$arrayParams["content"] = $strContent2; 
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
			$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strName = $consumer->GetResultValue("$$$ENTITYNAME$$$","NAME");
			$strContent = $consumer->GetResultValueInsecure("$$$ENTITYNAME$$$","CONTENT");

			
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("CONTENT = \"$strContent\"");
			if ($strName != $strName2)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($strContent != $strContent2)
			{
				$this->Trace("Error: item data mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("5. LIST");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["filter1"] = "name";
			$arrayParams["filter1_value"] = $strName2;
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
			if ($arrayList[0]["$$$ENTITYNAME$$$_ID"] != $this->m_strItemID)
			{
				$this->Trace("Error: item id mismatch");		
				return;
			}
			
			if ($arrayList[0]["NAME"] != $strName2)
			{
				$this->Trace("Error: item name mismatch");		
				return;
			}
			if ($arrayList[0]["CONTENT"] != SecureOutput($strContent2))
			{
				$this->Trace("Error: item data mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("6. DELETE ITEM");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
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
			$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
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
			$arrayParams["filter1"] = "name";
			$arrayParams["filter1_value"] = $strName2;
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
				$arrayParams["$$$entityname$$$_id"] = $this->m_strItemID;
				$arrayParams["command"] = "delete";
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace($consumer->GetServiceOutput());
				$this->Trace("");
			}
			
			return true;
		}
		
		
	}
	
	

		
