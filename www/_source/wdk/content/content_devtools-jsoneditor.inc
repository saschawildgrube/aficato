<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('devtools/jsoneditor');
			$module = new CJsonEditorModule($this->GetWebsite());
			$arrayItems['MODULE_JSONEDITOR'] = $module->RenderModule();
		}
	}
