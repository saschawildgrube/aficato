<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$icon = new CIconElement($this->GetWebsite(), 'default');
			$arrayItems['ELEMENT_ICON'] = $icon->RenderIcon('link');
		}
	}
	
