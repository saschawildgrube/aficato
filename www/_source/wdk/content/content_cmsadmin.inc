<?php

	class CMyContent extends CContentCMSAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->LoadStringResources("cmsadmin");
			$navigationtree = $this->GetWebsite()->GetNavigationTree("");
			$navigation = new CNavigationElement($this->GetWebsite(),"portfolio");
			$arrayItems["PORTFOLIO"]  = $navigation->RenderNavigationCurrentPathPortfolio($navigationtree);
		}

		function CallbackGetRequiredConditions()
		{
			return array("cmsadmin" => "INSUFFICIENT_PRIVILEGES");
		}				

		
	}
	
