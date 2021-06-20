<?php

	class CSortOptionElement extends CElement
	{
		private $m_linkicon;
		private $m_icon;
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$this->m_linkicon = null;
			$this->m_icon = null;
			parent::__construct(
				$website,
				"sortoption",
				$strStyleID,
				false,
				true);
		}
		
		function RenderSortOption(
			$strSortOptionParam,
			$strSortOption,
			$strSortOrderParam,
			$strSortOrder,
			$strToolTipAsc,
			$strToolTipDesc)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			if ($this->m_linkicon == null)
			{
				$this->m_linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			}
			if ($this->m_icon == null)
			{
				$this->GetWebsite()->IncludeElement("icon");
				$this->m_icon = new CIconElement($this->GetWebsite(),"default");
			}

			// ASC
			$arrayParams = array();
			$arrayParams[$strSortOptionParam] = $strSortOption;
			$arrayParams[$strSortOrderParam] = "asc";
			$strURL = $this->GetWebsite()->MakeURL($arrayParams);
			$strASC = $this->m_linkicon->RenderLinkIcon($strURL,"chevron-down",$strToolTipAsc);

			// ASC ACTIVE
			$strASC_ACTIVE = $this->m_icon->RenderIcon("chevron-circle-down",$strToolTipAsc);
			
			// DESC
			$arrayParams = array();
			$arrayParams[$strSortOptionParam] = $strSortOption;
			$arrayParams[$strSortOrderParam] = "desc";
			$strURL = $this->GetWebsite()->MakeURL($arrayParams);
			$strDESC = $this->m_linkicon->RenderLinkIcon($strURL,"chevron-up",$strToolTipDesc);

			// DESC ACTIVE
			$strDESC_ACTIVE = $this->m_icon->RenderIcon("chevron-circle-up",$strToolTipDesc);

		
			
			$arrayItems = array();
			switch ($strSortOrder)
			{
				case "asc":
				{
					$arrayItems["ASC"] 	= $strASC_ACTIVE;
					$arrayItems["DESC"]	= $strDESC;
					break;	
				}
				
				case "desc":
				{
					$arrayItems["ASC"] 	= $strASC;
					$arrayItems["DESC"]	= $strDESC_ACTIVE;
					
					break;	
				}
				
				case "":
				{
					$arrayItems["DESC"]	= $strDESC;
					$arrayItems["ASC"]	= $strASC;
					break;	
				}
				
				default:
				{
					$this->Error("Invalid sort order: \"$strSortOrder\"");
					break;	
				}
					
				
				
				
			}
		
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}


	}

