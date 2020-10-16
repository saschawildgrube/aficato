<?php



	class CSSLModule extends CModule
	{
		private $m_strContent;
	
		function __construct(
			CWebSite $website,
			$strContent = "")
		{
			$this->m_strContent = $strContent;
			parent::__construct(
				$website,
				"security/ssl");
		}
		
		function CallbackInit()
		{
			if ($this->GetWebsite()->GetProtocol() == "https")
			{
				$this->SetState("https");
			}
			else
			{
				$this->SetState("http");
				$this->SetOutputItem("SSLICON","");	
			}
			
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "http":
				{
					return $this->StateHttp();
				}				
				case "https":
				{
					return $this->StateHttps();
				}				
				default:
				{
					$this->Error("undefined state!");
				}
			}
		}
		
		
		function StateHttp()
		{
			$this->SetState("http");
			return "http";
		}

		
		function StateHttps()
		{
			$this->SetState("https");
			
			if ($this->m_strContent == "")
			{
				$this->IncludeElement("icon");
				$icon = new CIconElement($this->GetWebsite(),"default");
				$this->SetOutputItem("SSLICON",$icon->RenderIcon("ssl","?TID_TOOLTIP_SSL?"));
			}
			else
			{
				$this->IncludeElement("linkicon");
				$strLink = $this->GetWebsite()->MakeContentURL($this->m_strContent);
				$icon = new CLinkIconElement($this->GetWebsite(),"default");
				$this->SetOutputItem(
					"SSLICON",
					$icon->RenderLinkIcon($strLink,"ssl","?TID_TOOLTIP_SSL?"));
				
				
			}
			return "https";
		}

	
		
	}

