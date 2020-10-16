<?php

	class CLinkIconElement extends CElement
	{
		private $m_icon;
		
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$this->m_icon = null;			
			parent::__construct(
				$website,
				'linkicon',
				$strStyleID,
				false,
				true);
		}
		
		function RenderLinkIcon($strURL, $strIconID, $strToolTip = '', $bExternal = false)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if (strstr($strURL,'&amp;') == false)
			{
				$strURL = HtmlEncode($strURL);
			}
			
			if ($this->m_icon == null)
			{
				$this->GetWebsite()->IncludeElement('icon');
				$this->m_icon = new CIconElement($this->GetWebsite(),'default');  
			}
			$arrayItems['ICON'] = $this->m_icon->RenderIcon($strIconID);
			
			$arrayItems['URL'] = $strURL;
			$arrayItems['TOOLTIP'] = HtmlEncode($strToolTip);
			$arrayItems['EXTERNAL'] = ($bExternal)?(' target="_blank"'):('');
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

