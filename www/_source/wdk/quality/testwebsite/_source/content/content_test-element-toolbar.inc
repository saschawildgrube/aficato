<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
	
			$toolbarDef = new CToolbarDefinition();
			$toolbarDef->AddTool('http://www.example.com', 'link', 'Example.com');
			$toolbarDef->AddTool('http://www.example.com', 'link', '');
			$toolbarDef->AddTool('http://www.example.com', '', 'Example.com', true);
			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$arrayItems['ELEMENT_TOOLBAR'] = $toolbar->RenderToolbar($toolbarDef);

		}
	}
	
