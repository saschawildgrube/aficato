<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$icon = new CIconElement($this->GetWebsite(), 'default');
			$arrayItems['ELEMENT_ICON'] = $icon->RenderIcon('link');
		}
	}
	
