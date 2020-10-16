<?php



	class CWikiChaptersModule extends CModule
	{
		private $m_strLinkTemplateChapter;
		function __construct(
			CWebSite $website,
			$strInstanceID,
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/wikichapters",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayParams[$this->GetModuleVarName("chapter")] = "{CHAPTERID}";
			$arrayParams[$this->GetModuleVarNameState()] = "chapter";
			$strLinkTemplateChapter = $this->GetWebsite()->MakeURL($arrayParams);
			$strLinkTemplateChapter = ReplaceString($strLinkTemplateChapter,"%7B","{"); 
			$strLinkTemplateChapter = ReplaceString($strLinkTemplateChapter,"%7D","}");
			$this->m_strLinkTemplateChapter = $strLinkTemplateChapter;
			$this->IncludeElement("wiki");
			return true;
		}
		
		

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "chapter":
				{
					return $this->StateChapter();	
				}
				case "toc":
				default:	
				{
					return $this->StateTOC();
				}
			}
		}
		
		
		
		function StateChapter()
		{
			$this->Trace("StateChapter");
			
			$strChapterID = $this->GetModuleVar("chapter");
			if ($strChapterID == "" || $strChapterID == "toc")
			{
				return $this->StateTOC();	
			}
						
			$strContent = $this->GetModuleConfig("content");

			$navigationtree = new CNavigationTree();

			$wiki = new CWikiElement($this->GetWebSite(),"default");
			
			$this->SetOutputItem(
				"CHAPTER",
				$wiki->RenderWiki(
					$strContent,
					"",
					$strChapterID,
					$this->m_strLinkTemplateChapter,
					$navigationtree));
	
			
			
			$toolbarDef = new CToolbarDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "toc";
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$toolbarDef->AddTool($strLink, "list", "?TID_WIKICHAPTERS_TOOL_TOC?");

			$previtem = $navigationtree->GetPreviousItem();
			if ($previtem != null)
			{
/*				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "chapter";
				$arrayParams[$this->GetModuleVarName("chapter")] = "prev";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);*/
				$toolbarDef->AddTool($previtem->m_strLink, "back", "?TID_WIKICHAPTERS_TOOL_PREVIOUS?");
			}
			
			$nextitem = $navigationtree->GetNextItem();
			if ($nextitem != null)
			{
			/*
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "chapter";
			$arrayParams[$this->GetModuleVarName("chapter")] = "next";
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			*/
				$toolbarDef->AddTool($nextitem->m_strLink, "next", "?TID_WIKICHAPTERS_TOOL_NEXT?");
			}

			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$strOutput = $toolbar->RenderToolbar($toolbarDef); 

			
	
			$this->SetOutputItem(
				"TOOLBAR",
				$toolbar->RenderToolbar($toolbarDef));
	
			return "chapter";
		}
		
		function StateTOC()
		{
			$this->Trace("StateTOC");
			
			$strWikiContent = $this->GetModuleConfig("content");
			
			$wiki = new CWikiElement($this->GetWebSite(),"default");
			$this->SetOutputItem(
				"TOC",
				$wiki->RenderWiki(
					$strWikiContent,
					"",
					"TOC",
					$this->m_strLinkTemplateChapter
					)
				);
	
			return "toc";
		}		
		


	}

