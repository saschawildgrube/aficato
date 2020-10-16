<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->SetBodyItem("a",'<div id="a">This is a body item!</div>');
			$this->GetWebsite()->SetBodyItem("b",'<div id="b">And another body item!</div>');
		}
	}
	
