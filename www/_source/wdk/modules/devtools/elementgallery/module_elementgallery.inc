<?php



	class CElementGalleryModule extends CModule
	{
		private $m_arrayElements;
	
		function __construct(
			CWebSite $website)
		{
			parent::__construct(
				$website,
				"devtools/elementgallery");
		}
		
		function CallbackInit()
		{
			if ($this->GetState() == "")
			{				
				$this->SetState("list");	
			}
			
			$this->GetWebsite()->LoadStringResources("wdk-elements");
			
			$arrayToolbarConfig = array(
				"list" => array("viewall"),
				"viewall" => array("list"),
				"view" => array("list","viewall")
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);

			$this->m_arrayElements = $this->GetWebsite()->GetAssemblyLine()->EnumerateElements();
 
 			$this->m_arrayElements = ArraySort($this->m_arrayElements);
			
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "list":
				{
					return $this->StateList();
				}				
				case "viewall":
				{
					return $this->StateViewAll();
				}				
				case "view":
				{
					return $this->StateView();
				}				
				default:
				{
					return $this->StateList();
				}
			}
		}
		
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();


			if ($this->IsToolVisible("list"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "list";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_ELEMENTGALLERY_TOOL_LIST?");
			}
			
			if ($this->IsToolVisible("viewall"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "viewall";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list-details", "?TID_ELEMENTGALLERY_TOOL_VIEWALL?");
			}			

			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}
		
		function StateList()
		{
			$this->SetState("list");
			
	
			// TOOLBAR
			$this->RenderToolbar();

			// LIST
			$link = new CLinkElement($this->GetWebsite(),"default");
			
			$arrayList = array();
			foreach ($this->m_arrayElements as $strElementID)
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "view";
				$arrayParams[$this->GetModuleVarName("elementid")] = $strElementID;
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$strLinkText = $this->LoadStringElement($strElementID,"TITLE");
				array_push($arrayList,$link->RenderLink($strLink,$strLinkText));
			}

			
			$list = new CListElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem("LIST",$list->RenderList($arrayList));
			
			return "list";
		}

		
		function StateViewAll()
		{
			$this->SetState("viewall");
				
			// TOOLBAR
			$this->RenderToolbar();

			// ELEMENTS
			$strElements = "";
			foreach ($this->m_arrayElements as $strElementID)
			{
				$strElement = $this->RenderElement($strElementID,true,false,false);
				if ($strElement == false)
				{
					return "error";	
				}
				
				$strElements .= $strElement;
			}
			$this->SetOutputItem("ELEMENTS",$strElements);
			
			return "viewall";
		}
		
		
		function StateView()
		{
			$this->SetState("view");
			$strElementID = $this->GetModuleVar("elementid");


			// TOOLBAR
			$this->RenderToolbar();
			
			// ELEMENT
			$strOutput = $this->RenderElement($strElementID);
			if ($strOutput == false)
			{
				return "error";	
			}
			$this->SetOutputItem("ELEMENT",$strOutput);
			
			return "view";
		}

	
	
	
	
	
		function GetSourceCode($strElementID)
		{
			$strSourceCodeFile = $this->GetWebsite()->GetAssemblyLine()->GetElementDemoCodeFilePath($strElementID);
			if (IsFile($strSourceCodeFile) == false)
			{
				$this->SetErrorMessageText("?TID_ELEMENTGALLERY_ERROR_NODEMOCODEFILE?");	
				return false;
			}
			$strSourceCode = FileRead($strSourceCodeFile);
			if ($strSourceCode == "")
			{
				$this->SetErrorMessageText("?TID_ELEMENTGALLERY_ERROR_DEMOCODEFILEEMPTY?");	
				return false;
			}
			return $strSourceCode;
		}

		function RenderElement(
			$strElementID,
			$bLinkToStateView = false,
			$bIncludeDemoCode = true,
			$bIncludeHtmlCode = true)
		{

			$this->Trace("RenderElement: $strElementID");

			$bDemoCode = false;

			// GET AND EVALUATE DEMO CODE
			$strSourceCode = $this->GetSourceCode($strElementID);
			if ($strSourceCode != false)
			{ 
				$ea = new CErrorAmnesty();
				$ea->Start();
				$strOutput = "";
				$result = eval($strSourceCode);
				$ea->End();
				if ($result === false)
				{
					$this->SetErrorMessageText("?TID_ELEMENTGALLERY_ERROR_DEMOCODEINVALID?");
					$this->Trace("Erroneous demo code:");
					$this->Trace($strSourceCode);
				}
				else
				{
					$bDemoCode = true;	
					$strOutput = $this->GetWebsite()->ReplaceText($strOutput);
				}
			}
			

			$this->IncludeElement("collection");
			$collection = new CCollectionElement($this->GetWebsite(),"default");
			$arrayCollection = array();


			// TITLE
			$strElementTitle = $this->LoadStringElement($strElementID,"TITLE");
			if ($bLinkToStateView == true)
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "view";
				$arrayParams[$this->GetModuleVarName("elementid")] = $strElementID;
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					
				$link = new CLinkElement($this->GetWebsite(),"default");
				$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
				
				$strLinkOutput = $link->RenderLink($strLink,$strElementTitle);
				$strLinkIconOutput = $linkicon->RenderLinkIcon($strLink,"view",$strElementTitle);
				
				$strTitleOutput = $strLinkOutput."&nbsp;".$strLinkIconOutput;
			}
			else
			{
				$strTitleOutput = $strElementTitle;
			}
			
			$arrayCollection[] = array(
				"element" => "header",
      	"headerlevel" => "1",
      	"header" => $strTitleOutput);


			

			if ($bDemoCode == true)
			{
				// ELEMENT DISPLAY
/*
				$arrayCollection[] = array(
					"element" => "header",
	      	"headerlevel" => "2",
	      	"header" => "?TID_ELEMENTGALLERY_TITLE_DISPLAY?");				
*/
/*				$arrayCollection[] = array(
					"element" => "separator");
*/

				$this->IncludeElement("example");
				$example = new CExampleElement($this->GetWebsite(),"default");

				$arrayCollection[] = array(
					"element" => "raw",
	      	"data" => $example->RenderExample($strOutput));
/*				$arrayCollection[] = array(
					"element" => "separator");
*/
			}
			
			// DESCRIPTION
			$strDescriptionText = $this->LoadStringElement($strElementID,"TEXT");
			if ($strDescriptionText != "")
			{
					$arrayCollection[] = array(
						"element" => "header",
		      	"headerlevel" => "2",
		      	"header" => "?TID_ELEMENTGALLERY_TITLE_DESCRIPTION?");
					$arrayCollection[] = array(
						"element" => "text",
		      	"text" => $strDescriptionText);
				
			}
		
			
			if ($bDemoCode == true)
			{
				// DEMO CODE		
				if ($bIncludeDemoCode == true)
				{
					$arrayCollection[] = array(
						"element" => "header",
		      	"headerlevel" => "2",
		      	"header" => "?TID_ELEMENTGALLERY_TITLE_SOURCECODE?");
					$arrayCollection[] = array(
						"element" => "console",
		      	"data" => $strSourceCode);
				}
				
				// HTML CODE
				if ($bIncludeHtmlCode == true)
				{
					$arrayCollection[] = array(
						"element" => "header",
		      	"headerlevel" => "2",
		      	"header" => "?TID_ELEMENTGALLERY_TITLE_HTMLCODE?");
					$arrayCollection[] = array(
						"element" => "console",
		      	"data" => $strOutput);
		  	}
		  }

			$strElement = $collection->RenderCollection($arrayCollection,2); 

			return $strElement;
		}

		function LoadStringElement($strElementID,$strType)
		{
			$strTID = "TID_".StringUpperCase($strType)."_ELEMENT_".StringUpperCase($strElementID);
			$strString = $this->GetWebsite()->LoadString($strTID);	
			if ($strString != "")
			{
				return $strString;
			}
			
			$this->GetWebsite()->LoadStringResources("element-".$strElementID);
			$strString = $this->GetWebsite()->LoadString($strTID);
			if ($strString != "")
			{
				return $strString;
			}
			
			if (StringUpperCase($strType) == "TITLE")
			{
				return $strElementID;	
			}
			return "";
		}




	}

