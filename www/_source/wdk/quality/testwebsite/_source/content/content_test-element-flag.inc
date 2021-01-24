<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->IncludeElement('flag');
			
			$flag = new CFlagElement($this->GetWebsite(), 'default');
						
			$arrayItems['ELEMENT_FLAG_DEU'] = $flag->RenderFlag('DEU');
			$arrayItems['ELEMENT_FLAG_FRA'] = $flag->RenderFlag('FRA');
		}
	}
	
