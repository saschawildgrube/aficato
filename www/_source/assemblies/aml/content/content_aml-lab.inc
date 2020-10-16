<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$navigationtree = $this->GetWebsite()->GetNavigationTree('');
			$navigation = new CNavigationElement($this->GetWebsite(),'portfolio');
			$arrayItems['PORTFOLIO']  = $navigation->RenderNavigationCurrentPathPortfolio($navigationtree);
		}
	}
	
