<?php

	require_once('env.inc');
	require_once(GetWDKDir().'wdk_website.inc');
	
	class C$$$WebSiteName$$$WebSite extends CWebSite
	{

		function __construct(
			$arrayParams)
		{
			
			$arrayMeta = array();
			$arrayMeta['version'] = '0.1'; 
			$arrayMeta['author'] = '$$$author$$$';
			$arrayMeta['copyright'] = '?TID_COPYRIGHT?';
			$arrayMeta['copyright_since'] = '$$$copyrightsince$$$';
			
			
			$arrayConfig = array();
			
			/*
				Session Mode
				
				For a website without a database backend there are limited options
				for session handling.
				
				Option 1: No sessions at all

				Without any session support all state variables are packed into each
				URL within the website.
				Due to the limitations in both browsers and servers the total amount
				of data that can be transferred between states is very limited.
				The downside is that all data is visible in GET requests and will
				hence be protocolized in the web server's access log.
			*/
			//$arrayConfig['session_mode'] = SESSION_MODE_NONE;
				
			/*
				Option 2: Cookies used as session storage
				
				All state variables are stored directly in a cookie. This is
				the classical approach.
				But handle with care: All data will be stored in the browser's
				cookie data files locally.
				Do not use this if users have to enter confidential information
				that is kept in variables over multiple states.
			*/
			$arrayConfig['session_mode'] = SESSION_MODE_LOCAL_COOKIE;
			
			/*
				Caching - if implemented - is always active. Unless explicitly
				deactivated.
			*/			
			$arrayConfig['cache_deactivate'] = true;
			
			/*
				The 'cache_allow_nocache' setting controls whether caching
				can be deactivated via parameter.
			*/			
			$arrayConfig['cache_allow_nocache'] = true;
			
			/*
				Use this setting to control if a warning message should be displayed 
				to indicate that the website is still under construction.
			*/
			$arrayConfig['underconstruction'] = true;
			
			/*
				You can use this setting to explicitly allow a browser to embedd the
				website into an iframe if this is intended.
			*/
			$arrayConfig['iframe_allow'] = false;
			
			/*
				Use this setting to track missing translations.
				Since the additional check may impact performance the setting should
				be deactivated if no longer needed.
			*/
			$arrayConfig['check_unresolvedtid'] = true;

			/*
				Use this setting to track invalid text IDs.
				Since the additional check may impact performance the setting should
				be deactivated if no longer needed.
			*/
			$arrayConfig['check_invalidtid'] = true;

			/*
				Use this setting to track php messages or other unintended output
				in the output stream buffer.
				Since the check may impact performance the setting should
				be deactivated once the check is no longer needed.
			*/
			$arrayConfig['check_unintendedoutput'] = true;
			
			/*
				The following settings control locale support of the website.
				'languages' is the list of supported screen text languages.
				'countries' is the list of country specific locales that are supported.
				This controls time and date formatting and currency options.
				'defaultcountry' sets the country locale settings that should be used
				if no country is specified.
			*/			
			$arrayConfig['languages'][] = 'EN'; 
			$arrayConfig['defaultcountry'] = 'USA';
			$arrayConfig['countries'][] = 'GBR';
			$arrayConfig['countries'][] = 'USA';
			
			
			/*
				The layouts setting may be used to let a user choose between different
				layouts:
			*/
			$arrayConfig['layouts'][] = '$$$websitename$$$';
			
			/*
				The protocol setting controls whether an application can be accessed via
				http or https or both. The user will be redirected if an unsupported
				protocol is used.
			*/
			$arrayConfig['protocols'][] = 'http';
			$arrayConfig['protocols'][] = 'https';
			
			/*
				The administrator's email used e.g. for error emails:
			*/			
			$arrayConfig['admin_email'] = GetAdminEmail();
			
			/*
				This setting controls the support of seo friendly urls:
			*/
			$arrayConfig['seourl'] = '$$$seourlparam$$$';
			
			/*
				This settings controls whether OpenGraph headers should be set.
				Overload the function OnGetOpenGraphImageURL() to set the image.
			*/
			$arrayConfig['opengraph'] = true;
						
			$arrayConfig['csvformat'] = 'excel';
			
			parent::__construct(
				GetRootURL(),
				GetSourceDir(),
				GetDocumentRootDir(),
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				'website_$$$websitename$$$');
					
		}
		
		function CallbackInitAssemblyLine(&$assemblyLine)
		{
			/* 
				To use more assemblies, add the following code for each assembly:
			
			$assembly = new CAssembly($this,'assemblyfoldername');
			$assemblyLine->AddAssembly($assembly);
	
			*/

			parent::CallbackInitAssemblyLine($assemblyLine);
		}
		
		function CallbackInitRoutingMap()
		{
			parent::CallbackInitRoutingMap();
			$this->LoadRoutingMap('default');
		}
		
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			
			/*
				Add initialization code here!
			*/
			
			$this->RegisterGenericOutputItem('GOOGLEURL',array('search'));
			$this->SystemLog('INFORMATIONAL','REQUEST', $this->GetContent());

			$this->SetLayout('$$$websitename$$$');


			// DISPLAY DATETIME
			$this->IncludeElement('datetime');
			$datetime = new CDateTimeElement($this,'default');
			$this->SetOutputItem('DATETIME',$datetime->RenderDateTime(GetTimeNow()));	

			// LANGUAGE SELECTION
			$this->IncludeModule('user/languageselection');
			$languageselectionModule = new CLanguageSelectionModule($this,'index');
			$this->SetOutputItem('LANGUAGESELECTION',$languageselectionModule->RenderModule('toolbar'));	



			return $bResult;
		}
		
		function GetSiteName()
		{
			return '$$$WebSiteNameVerbose$$$';
		}
		
		function GetSiteTeamEmail()
		{
			return 'info@'.GetMailDomain();
		}

		function GetSiteTeamName()
		{
			return '$$$siteteam$$$';
		}
	
		function CallbackPreContent()
		{
			/*
				Called before the actual content rendering takes place.
			*/
		}

		function CallbackPostContent()
		{
			/*
				Called after the actual content rendering takes place.
			*/			
		}
		
		function CallbackRenderContentException()
		{
			/*
				This function is called if a requested content is not found.
				If you were implementing a wiki, this would be the place to create a
				new page.
			*/
			return parent::CallbackRenderContentException();
		}
		
		function CallbackRenderSpecialContent($strContentID)
		{
			/*
				Before trying to load the content from a file, this function is called.
				If the function returns an empty string, the content is loaded from a file.
				If the function returns anything else, the returned string is used as the content.
			*/
			
			/*
				The following code demonstrates a way to override the loading of content
				based on given parameters.
			*/
			if ($this->GetVar('disclaimerconfirmed') != '1')
			{
				$this->IncludeModule('compliance/disclaimer');
				$disclaimer = new CDisclaimerModule($this);
				return $disclaimer->RenderModule('default');
			}
			return parent::CallbackRenderSpecialContent($strContentID); 
		}

		function CallbackRenderGenericOutputItem($strItem, $arrayAttributes)
		{
			switch ($strItem)
			{
			case 'GOOGLEURL':
				return 'http://www.google.de/search?q='.ArrayGetValue($arrayAttributes,'search');

			default:
				return parent::CallbackRenderGenericOutputItem($strItem, $arrayAttributes);
			}
		}
		

		function CallbackRenderNavigation($strOutput)
		{
			/*
				This function is called after content rendering is complete.
				This implementation replaces different tags from the framework layout
				with corresponding navigation elements.
			*/
			
			$navigationtree = new CNavigationTree();
			$navigationtree->LoadNavigationTree($this,'');
			$navigationtree->SetCurrentContext($this->GetContent());
			$navigationtree->SetContextParameterName('content');

			if (FindString($strOutput,'{NAVIGATION_HORIZONTALPATH}') != -1)
			{
				$navigation = new CNavigationElement($this,'horizontalpath');
				$this->SetOutputItem('NAVIGATION_HORIZONTALPATH',$navigation->RenderNavigationHorizontalPath($navigationtree));
	    }
         
			if (FindString($strOutput, '{NAVIGATION_VERTICALPATH}') != -1)
			{ 
				$navigation = new CNavigationElement($this,'verticalpath');
				$this->SetOutputItem('NAVIGATION_VERTICALPATH',
					$navigation->RenderNavigationVerticalPath($navigationtree,0));
      }
         
			if (FindString($strOutput, '{NAVIGATION_CURRENTPATH}') != -1)
			{ 
				$navigation = new CNavigationElement($this,'currentpath');
				$this->SetOutputItem('NAVIGATION_CURRENTPATH',
					$navigation->RenderNavigationCurrentPath($navigationtree));
			}
			
			if (FindString($strOutput, '{NAVIGATION_VERTICALCURRENTPATH}') != -1)
			{ 
				$navigation = new CNavigationElement($this,'verticalcurrentpath');
				$this->SetOutputItem('NAVIGATION_VERTICALCURRENTPATH',
					$navigation->RenderNavigationVerticalCurrentPath($navigationtree));
			}
         
			if (FindString($strOutput, '{NAVIGATION_FOOTER}') != -1)
			{
				$navigationtree_footer = new CNavigationTree();
				$navigationtree_footer->LoadNavigationTree(
					$this,
					'footer');
				$navigationtree_footer->SetCurrentContext($this->GetContent());
				$navigationtree_footer->SetContextParameterName('content');
         		
				$navigation = new CNavigationElement($this,'footer');
				$this->SetOutputItem('NAVIGATION_FOOTER',$navigation->RenderNavigationHorizontalPath($navigationtree_footer));
			}
		}




		function CallbackCheckCondition($strCondition)
		{
			/*
				Whenever a component uses the CWebsite::CheckCondition() to find out if
				a feature or option is available to a given user, this callback function
				is invoked.
				The parent implementation in CWebsite::CallbackCheckCondition() alsways returns
				false.
				CUsersWebSite::CallbackCheckCondition() on the other hand will check
				the users individual privileges.
				You can also use this function to enable or disable features globally or
				apply additional checks before granting a user access to specific features.
			
			*/
			return parent::CallbackCheckCondition($strCondition);	
		}
		
		
		function CallbackGetDateLocalFormat($strCountryID)
		{
			/*
				Use this function to override defaults defined in GetDateLocalFormat().
				return an empty string or remove the function completely to use defaults.
				
				The implementation below uses the ISO format for the USA.
			*/
			switch ($strCountryID)
			{
			case 'USA':
				return '%Y-%m-%d';
			default:
				return '';
			}
		}

		function CallbackGetTimeLocalFormat($strCountryID)
		{
			/*
				Use this function to override defaults defined in GetTimeLocalFormat().
				return an empty string or remove the function completely to use defaults.
				
				The implementation below defaults to the 24 hours clock format for the USA
				instead of the 12 hours clock format.
			*/
			switch ($strCountryID)
			{
				case 'USA':				
					return '%H:%M:%S';
				default:
					return '%I:%M:%S%P';
			}
		}
		
		
		function CallbackCreateImage(&$image,$strImageID)
		{
			/*
				This function is the actual command handler for the 'image' command.
				Whenever an image is requested, the callback function is invoked.
				If the parent implementation is called, the website will try to load an image file
				from the file system, and if it can't be found it will render an error image.
				
				The source code below demonstrates how to implement the function. It does the
				same like CWebsite::RenderErrorImage() for demonstration purposes.
			*/
			if ($strImageID == 'test')
			{			
				if ($image != null)
				{
					imagedestroy($image);	
				}
				$nWidth = 20;
				$nHeight = 20;
				
				$image = imagecreate($nWidth,$nHeight);
				$colorBackground =  imagecolorallocate ( $image, 255, 255, 255);
				$colorForeground =  imagecolorallocate ( $image, 255, 0, 0);
				imagefilledrectangle($image, 0, 0, $nWidth, $nHeight, $colorBackground);
				imageline ($image , 3, 3 , $nWidth-3, $nHeight-3, $colorForeground );
				imageline ($image , $nWidth-3, 3 , 3, $nHeight-3, $colorForeground );
				return true;
			}
			
			return parent::CallbackCreateImage($image,$strImageID);
		}

	}
	

