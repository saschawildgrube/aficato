<?php

	class CLinkImageElement extends CElement
	{
		private $m_icon;
		
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$this->m_image = null;			
			parent::__construct(
				$website,
				'linkimage',
				$strStyleID,
				false,
				true);
		}
		
		function RenderLinkImage($strURL, $strImageURL, $strToolTip = '', $bExternal = false)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if (strstr($strURL,'&amp;') == false)
			{
				$strURL = HtmlEncode($strURL);
			}
			
			if ($this->m_image == null)
			{
				$this->GetWebsite()->IncludeElement('image');
				$this->m_image = new CImageElement($this->GetWebsite(),'default');  
			}
			$arrayItems['IMAGE'] = $this->m_image->RenderImage($strImageURL,$strToolTip);
			
			$arrayItems['URL'] = $strURL;
			$arrayItems['TOOLTIP'] = HtmlEncode($strToolTip);
			$arrayItems['EXTERNAL'] = ($bExternal)?(' target="_blank"'):('');
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

