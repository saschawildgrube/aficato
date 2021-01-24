<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			
			$arrayMessages = array();
			$arrayMessages["info"] = array();
			$arrayMessages["info"][] = "This is an info message informing users about a successful operation.";
			$arrayMessages["warning"] = array();
			$arrayMessages["warning"][] = "This is a warning message that indicates that something did not work exactly as intended.";
			$arrayMessages["error"] = array();
			$arrayMessages["error"][] = "This is an error message that should alert a user that whatever was requested did not work at all.";
			
			$messagestack = new CMessageStackElement($this->GetWebsite(),"default");
			$arrayItems["MESSAGESTACK"] = $messagestack->RenderMessageStack($arrayMessages);
			
			 
			
			
			
			
			
			$arrayErrors = array();
			$formDef = new CFormDefinition();
			
			$formDef->SetURL(
			   $this->GetWebsite()->GetRootURL(),
			   $this->GetWebsite()->GetSessionAwareArrayVars());

			$formDef->AddControlStatic(
			   "STATIC1",
			   "This form demonstrates a typical form with a selection of controls. It should be considered that some optical aspects are controlled by the browser or operating system.");
			   
			$formDef->AddControlReadOnly(
			   "READONLY1",
			   "A value that can't be edited",
			   42);
			
			$formDef->AddControlText(
			   "TEXT1",
			   "text1",
			   "Text",
			   $this->GetVar("text1"),
			   40,
			   40);

			$formDef->AddControlText(
			   "TEXT1RO",
			   "text1",
			   "Text (read only)",
			   $this->GetVar("text1"),
			   40,
			   40,
			   true);


			$formDef->AddControlTextArea(
			   "TEXTAREA1",
			   "textarea1",
			   "Text Area",
			   $this->GetVar("textarea1"),
			   40,
			   5);			


			$arrayOptions = array();
			$arrayOptions["option1"] = "Option 1";
			$arrayOptions["option2"] = "Option 2";
			$arrayOptions["option3"] = "Option 3";
			
			$formDef->AddControlSelect(
			   "SELECT1",
			   "select1",
			   "Selection Dropdown",
			   $arrayOptions,
			   $this->GetVar("select1"));

			$formDef->AddControlRadio(
			   "RADIO1",
			   "radio1",
			   "Radio Buttons",
			   $arrayOptions,
			   $this->GetVar("radio1"));
			
			$formDef->AddControlButtonSubmit("Submit Button");
			
			$form = new CFormElement(
				$this->GetWebsite(),
			  "default");
			   
			$arrayItems["FORM"] = $form->RenderForm(
				$formDef,
	      $this->GetWebsite()->GetAllVars(),
	      $arrayErrors);

			
			
			$arrayList = array();
			$arrayList[] = "Apples";
			$arrayList[] = "Oranges";
			$arrayList[] = "Bananas";
			$arrayList[] = "Grapefruit";
			$arrayList[] = "Mango";
			
			$list = new CListElement($this->GetWebsite(),"default");
			$arrayItems["LIST"] = $list->RenderList($arrayList);
			
			$this->IncludeElement("numberedlist");
			$numberedlist = new CNumberedListElement($this->GetWebsite(),"default");
			$arrayItems["NUMBEREDLIST"] = $numberedlist->RenderNumberedList($arrayList);
			
			
			
			
			
			
			
			$toolbarDef = new CToolbarDefinition();
			$toolbarDef->AddTool("http://www.example.com", "list", "List");
			$toolbarDef->AddTool("http://www.example.com", "listdetail", "Thumbnail List");
			$toolbarDef->AddTool("http://www.example.com", "export", "Export");
			$toolbarDef->AddTool("http://www.example.com", "modify", "Modify");
			$toolbarDef->AddTool("http://www.example.com", "view", "View");
			$toolbarDef->AddTool("http://www.example.com", "url", "Link");
			$toolbarDef->AddTool("http://www.example.com", "delete", "Delete");
			$toolbarDef->AddTool("http://www.example.com", "refresh", "Refresh");
			$toolbarDef->AddTool("http://www.example.com", "help", "Help");
			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$arrayItems["TOOLBAR"] = $toolbar->RenderToolbar($toolbarDef); 
			
			
			
			
			
			
			
			
			
			$itemmanagerdata = new CItemManagerData();

			$formDef = new CFormDefinition();
			$formDef->SetURL(
				$this->GetWebsite()->GetRootURL(),
				$this->GetWebsite()->GetSessionAwareArrayVars());

			$strLinkReset = $this->GetWebsite()->MakeURL(array());
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			$strLinkIconHtml = $linkicon->RenderLinkIcon(
				$strLinkReset,
				"undo",
				"Reset all filter criteria");
			$formDef->AddControlStatic(
				"FILTERRESET",
				$strLinkIconHtml);

			$formDef->AddControlText(
				"FILTERSEARCH",
				"search",
				"Search",
				$this->GetVar("search"),
				20,
				20);

			$formDef->AddControlSelect(
				"FILTEROPTIONS",
				"options",
				"Filter",
				$arrayOptions,
				$this->GetVar("options"));

			$formDef->AddControlButtonSubmit("Search");
			
			$itemmanagerdata->SetFilterOptionsForm(
				$formDef,
				"horizontal",
				"");





			
			$strFilterOptionTab = $this->GetWebsite()->GetVar("itemmanager_filteroptions_tab");
			if ($strFilterOptionTab == "")
			{
			   $strFilterOptionTab = "all";   
			}
			
			$arrayTable = array();
			for ($nRowIndex = 0; $nRowIndex < 10; $nRowIndex++)
			{
			   if (   
			         ($strFilterOptionTab == "all")
			      || (($strFilterOptionTab == "even") && (($nRowIndex % 2) == 0))
			      || (($strFilterOptionTab == "uneven") && (($nRowIndex % 2) != 0))
			      )
			   {
			      $arrayRow = array();
			      for ($nColIndex = 1; $nColIndex <= 10; $nColIndex++)
			      {
			         $arrayRow["COL$nColIndex"] = "Row$nRowIndex Col$nColIndex";
			      }
			      $arrayTable[$nRowIndex] = $arrayRow;
			   }
			}
			$itemmanagerdata->SetItemList($arrayTable);

			
			$itemmanagerdata->AddColumn("COL1","Column 1","col1",true);
			$itemmanagerdata->AddColumn("COL2","Column 2","col2");
			$itemmanagerdata->AddColumn("COL3","Column 3","");
			$itemmanagerdata->AddColumn("COL4","Column 4","",true,"primary");
			$itemmanagerdata->AddColumn("COL5","Column 5","",false,"primary");
			$itemmanagerdata->AddColumn("COL6","Column 6","",false,"secondary");
			$itemmanagerdata->AddColumn("COL7","Column 7","",false,"secondary");
			$itemmanagerdata->AddColumn("COL8","Column 8","",true,"tertiary");
			$itemmanagerdata->AddColumn("COL9","Column 9","",true,"tertiary");
			$itemmanagerdata->AddColumn("COL10","Column 10","col10",false,"tertiary");
			
			$arrayColumnTabs = array(
			   "primary" => "Primary",
			   "secondary" => "Secondary",
			   "tertiary" => "Tertiary",
			   "unused" => "Unused");
			 
			$itemmanagerdata->SetColumnsTabNavigation(
			   $this->GetWebsite()->GetVar("itemmanager_columns_tab"),
			   "itemmanager_columns_tab",
			   $arrayColumnTabs);
			
			$itemmanagerdata->SetSortOptions(
			   "itemmanager_sort_option",
			   $this->GetVar("itemmanager_sort_option"),
			   "itemmanager_sort_order",
			   $this->GetVar("itemmanager_sort_order")
			   );

			$itemmanagerdata->SetPageNavigation(
			   $this->GetWebsite()->GetVar("itemmanager_offset"),
			   10,
			   "itemmanager_offset",
			   20);

			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			
			$arrayItems["ITEMMANAGER"] = $itemmanager->RenderItemManager($itemmanagerdata); 
			
			
		}
	}
	
