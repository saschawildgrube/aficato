<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		
		private $m_strWebservice;
		private $m_consumer;
		private $m_strItemID;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			$this->m_strWebservice = "demo/databasedemo";
			parent::__construct("DATABASEDEMO web service - Sort order",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			return parent::CallbackInit();	
		}	
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->CallbackCleanup();
			$arrayItemIDs = array();
			
		
			/*

			0. Remove all existing items
			
			1. Add without sort order (at end)
			2. Add with sort order = 0 (at start)
			3. Add without sort order (again at end)
			4. List and check
			
			5. Add with sort order = 1 (at center)
			6. Add with sort order = 666 (at the end)
			7. List and check 

			8. Set item from center to start
			9. Set item from center to end
			10. List and check 

			11. Set item from start to end
			12. Set item from start to center
			13. List and check 

			14. Set item from end to start
			15. Set item from end to center
			16. List and check 
			
			17. Set item from center to the left
			18. Set item from center to the right
			19. List and check 

			20. Delete from center
			21. List and check 
			
			22. Delete from end
			23. List and check 

			24. Delete from start
			25. List and check 
			
			*/

			
			$this->Trace("1. Add without sort order (at end)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = "AAA";
			$arrayParams["command"] = "add";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$arrayItemIDs["AAA"] = $this->m_consumer->GetResultValue("NEW_ITEM_ID");
			$this->Trace("");
			
			$this->Trace("2. Add with sort order = 0 (at start)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = "BBB";
			$arrayParams["item_sortorder"] = "0";
			$arrayParams["command"] = "add";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$arrayItemIDs["BBB"] = $this->m_consumer->GetResultValue("NEW_ITEM_ID");			
			$this->Trace("");

			$this->Trace("3. Add without sort order (again at end)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = "CCC";
			$arrayParams["command"] = "add";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$arrayItemIDs["CCC"] = $this->m_consumer->GetResultValue("NEW_ITEM_ID");
			$this->Trace("");
					
			$this->Trace("4. List and check");
			$arrayExpected = array("BBB","AAA","CCC");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}
			
			
			
			$this->Trace("5. Add with sort order = 1 (at center)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = "DDD";
			$arrayParams["item_sortorder"] = "1";
			$arrayParams["command"] = "add";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$arrayItemIDs["DDD"] = $this->m_consumer->GetResultValue("NEW_ITEM_ID");
			$this->Trace("");
			
		
			
			$this->Trace("6. Add with sort order = 666 (at the end)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_name"] = "EEE";
			$arrayParams["item_sortorder"] = "666";
			$arrayParams["command"] = "add";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$arrayItemIDs["EEE"] = $this->m_consumer->GetResultValue("NEW_ITEM_ID");
			$this->Trace("");
			
			$this->Trace("7. List and check");
			$arrayExpected = array("BBB","DDD","AAA","CCC","EEE");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}



			$this->Trace("8. Set item from center to start");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["AAA"];
			$arrayParams["item_sortorder"] = "0";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
			
			$this->Trace("8.1. List and check");
			$arrayExpected = array("AAA","BBB","DDD","CCC","EEE");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}	
			
			$this->Trace("9. Set item from center to end");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["DDD"];
			$arrayParams["item_sortorder"] = "666";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			
			
			$this->Trace("10. List and check");
			$arrayExpected = array("AAA","BBB","CCC","EEE","DDD");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}		
			
						
			
			$this->Trace("11. Set item from start to end");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["AAA"];
			$arrayParams["item_sortorder"] = "666";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
			$this->Trace("12. Set item from start to center");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["BBB"];
			$arrayParams["item_sortorder"] = "2";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			
			
			$this->Trace("13. List and check ");
			$arrayExpected = array("CCC","EEE","BBB","DDD","AAA");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}					
			
			
			
			$this->Trace("14. Set item from end to start");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["AAA"];
			$arrayParams["item_sortorder"] = "0";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
			$this->Trace("15. Set item from end to center");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["DDD"];
			$arrayParams["item_sortorder"] = "2";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			

			$this->Trace("16. List and check ");
			$arrayExpected = array("AAA","CCC","DDD","EEE","BBB");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}					
						
			
			
	
			$this->Trace("17. Set item from center to the left");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["DDD"];
			$arrayParams["item_sortorder"] = "1";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
			$this->Trace("18. Set item from center to the right");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["CCC"];
			$arrayParams["item_sortorder"] = "3";
			$arrayParams["command"] = "set";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			

			$this->Trace("19. List and check ");
			$arrayExpected = array("AAA","DDD","EEE","CCC","BBB");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}								
			
			
			
			$this->Trace("20. Delete from center");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["EEE"];
			$arrayParams["command"] = "delete";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			

			$this->Trace("21. List and check ");
			$arrayExpected = array("AAA","DDD","CCC","BBB");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}										
			
			$this->Trace("22. Delete from end");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["BBB"];
			$arrayParams["command"] = "delete";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			

			$this->Trace("23. List and check ");
			$arrayExpected = array("AAA","DDD","CCC");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}										

			$this->Trace("24. Delete from start");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["item_id"] = $arrayItemIDs["AAA"];
			$arrayParams["command"] = "delete";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			

			$this->Trace("25. List and check ");
			$arrayExpected = array("DDD","CCC");
			if ($this->CheckExpectedSortOrder($arrayExpected) == false)
			{
				return;
			}										
		

			$this->SetResult(true);
		}
		
		function CheckExpectedSortOrder($arrayExpected)
		{
			$this->Trace("CheckExpectedSortOrder");
			$this->Trace($arrayExpected);
			
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["offset"] = "0";
			$arrayParams["blocksize"] = "100";
			$arrayParams["sort1"] = "item_sortorder";
			$arrayParams["sort1_order"] = "asc";
			$arrayParams["command"] = "list";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return false;	
			}
			$arrayList = $this->m_consumer->GetResultList();
			
			$nIndex = 0;
			foreach ($arrayExpected as $strExpectedName)
			{
				$arrayItem = ArrayGetValue($arrayList,$nIndex);
				$strName = ArrayGetValue($arrayItem,"ITEM_NAME");
				if ($strName != $strExpectedName)
				{
					$this->Trace("Item \"$strName\" is not at the right position!");		
					return false;
				}
				$nIndex++;
			}
			
			$nIndex = 0;
			foreach ($arrayList as $arrayItem)
			{
				$strName = ArrayGetValue($arrayItem,"ITEM_NAME");
				$nSortOrder = intval(ArrayGetValue($arrayItem,"ITEM_SORTORDER"));
				if ($nSortOrder != $nIndex)
				{
					$this->Trace("Item \"$strName\" does not have the expected sort order value!");		
					return false;
				}
				$nIndex++;
			}
			
			return true;
		}
		
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			
			
			$this->Trace("DELETE ALL ITEMS");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["offset"] = "0";
			$arrayParams["blocksize"] = "100";
			$arrayParams["command"] = "list";
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($this->m_consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
				return false;	
			}
			$arrayList = $this->m_consumer->GetResultList();
			foreach($arrayList as $arrayItem)
			{
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["item_id"] = ArrayGetValue($arrayItem,"ITEM_ID");
				$arrayParams["command"] = "delete";
				$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			}
			$this->Trace("");
			return true;
		}
		
		
	}
	
	

		
