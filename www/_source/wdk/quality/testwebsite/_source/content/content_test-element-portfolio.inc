<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->IncludeElement("portfolio");
			$portfolio = new CPortfolioElement($this->GetWebsite(),"default");
			
			$arrayTeasers = array();
			
			$arrayTeaser["imageurl"] = '{IMAGEURL id="test"}';
			$arrayTeaser["title"] = "Title One";
			$arrayTeaser["text"] = "This is text number one.";
			$arrayTeaser["linktext"] = "Link One";
			$arrayTeaser["linkurl"] = "http://www.example.com";
			$arrayTeaser["external"] = true;
			$arrayTeasers[] = $arrayTeaser;

			$arrayTeaser["imageurl"] = '{IMAGEURL id="test"}';
			$arrayTeaser["title"] = "Title Two";
			$arrayTeaser["text"] = "This is text number two.";
			$arrayTeaser["linktext"] = "Link Two";
			$arrayTeaser["linkurl"] = "http://www.example.com";
			$arrayTeaser["external"] = false;
			$arrayTeasers[] = $arrayTeaser;
			
			$arrayItems["ELEMENT_PORTFOLIO"] = $portfolio->RenderPortfolio($arrayTeasers);
		}
	}
	
