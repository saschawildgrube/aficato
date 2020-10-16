<?php

	require_once('env.inc');
	require_once(GetWDKDir().'wdk_website_dynamic.inc');
	
	class C$$$WebSiteName$$$WebSite extends CDynamicWebSite
	{

		public $m_moduleSignin;
		public $m_moduleSigninOneStep;
		public $m_moduleSignOut;
		public $m_moduleLogin;
		public $m_moduleUserEmail;


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
				Security is priority number one!
				The following settings enable to balance protection and user convenience.
			*/
			
			/*
				Setting 1: The session id strategy:
				
				A dynamic website requires a session which is identified by a session id.
				This session id must be transported to the server by the browser in each request.
				
				Option 1: Session Cookies
				The session id can be stored in a cookie.
				The advantage is that URLs do not contain the session ids - this protects the
				application against session highjacking.
				The disadvantage is a potential vulnerability against CSRF attacks.
				Another minor downside is that a link cannot be copied from one browser to another.
			*/
			//$arrayConfig['session_mode'] = SESSION_MODE_SERVER_COOKIE;
			
			/*
				Option 2: Session ids in URLs
				The session id can be embedded into all links and forms and will hence be
				transferred via GET or POST request parameter.
				The advantage is that CSRF attacks are not possible.
				The disadvantage is that the application is potentially vulnerable to session
				highjacking since the session is transferred unencryptedly even in HTTPS requests.
				Another advantage during development and incident analysis is that a URL can be
				shared within a team across different browsers.
			*/
			//$arrayConfig['session_mode'] = SESSION_MODE_SERVER_URL;       
			
			/*
				Option 3: Session IDs are split between URLs and cookie.
				The session id is split into two parts. The first part is embedded into URLs
				and POST request parameters. The second part is stored in a cookie.
				This method combines the advantages of URL and cookie based session ids.
				CSRF attacks are unlikely and session highjacking is impossible when using https.
				Here again: the link can't be shared across browsers or users - but this should not
				be relevant in production.
				This is the recommended option!
			*/			
			$arrayConfig['session_mode'] = SESSION_MODE_SERVER_COMBINED;   

			/*
				Setting 2: Session to IP binding
				If set to 'true' the IP address of the application's user is stored in the
				session and checked in each request.
				This provides additional protection against session riding and session highjacking.
				'true' is the recommended option.
			*/
			$arrayConfig['session_ip'] = true;

			/*
				Setting 3: CSRF defuse
				CSRF protection is implemented using the 'Synchronizer Token Pattern'.
				Refer to https://www.owasp.org/index.php/Cross-Site_Request_Forgery_(CSRF)_Prevention_Cheat_Sheet
				In WDK this pattern is implemented in a more strict approach:
				A synchronizer token is embedded in every request (not just those
				considered as	'sensitive').
				The token is checked and renewed during each request.
				The advantage is that this (in combination with the combined session id option)
				does not leave any known attack vector for CSRF.
				The disadvantage is that users must stay in one browser window or tab.
				This may annoy users who want to 'fork' and continue to browse a website
				in two tabs.
				And, the back button is broken as a result, too.
				Users will see error messages in those cases.
				This is the recommended option for applications operating with personal,
				financial, medical, military, or governmental data or trade secrets, or
				any other application that processes highly sensible information.
				It may be deactivated (be setting the option 'true') for casual web
				applications in which non-critial information is processed and the
				user experience is the main	success factor.
			*/
			$arrayConfig['csrf_defuse'] = false;  
			
			
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
			$arrayConfig['fallbacklanguage'] = 'EN'; 
			$arrayConfig['countries'][] = 'GBR';
			$arrayConfig['countries'][] = 'USA';
			$arrayConfig['defaultcountry'] = 'USA';
			
			
			/*
				The layouts setting may be used to let a user choose between different
				layouts:
			*/
			$arrayConfig['layouts'][] = '$$$websitename$$$';
			
			/*
				The protocol setting controls whether an application can be accessed via
				http or https or both. An error message is displayed when using an unsupported
				protocol.
			*/
			$arrayConfig['protocols'][] = 'http';
			$arrayConfig['protocols'][] = 'https';
			
			/*
				The administrator's email used e.g. for error emails:
			*/			
			$arrayConfig['admin_email'] = GetAdminEmail();
			
			/*
				These privileges are given to users that are created in the sign-in process
				by default:
			*/
			$arrayConfig['defaultprivileges'][] = 'DEVTOOLS';
			
			/*
				These privileges can be granted or revoked in the user management:
			*/
			$arrayConfig['privileges'][] = 'USERADMIN';
			$arrayConfig['privileges'][] = 'SYSTEMADMIN';
			
			/*
				This defines the rules for new passwords chosen by the user:
			*/
			$arrayConfig['passwordpolicy'] = array(
				'minimumlength' => 3,
				'requiredcharsnumeric' => true,
				'requiredcharsalpha' => false,
				'requiredcharsalphauppercase' => false,
				'requiredcharsalphalowercase' => false
				);
		
			/*
				This setting controls the support of seo friendly urls:
			*/
			$arrayConfig['seourl'] = '$$$seourlparam$$$';

		
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
			
			/*
				This registers a 'generic output item'.
				Output items are tags that look like this '{ITEM}'. They begin and end
				with curly bracets and are always uppercase.
				They are replaced with content during rendering the website.
				A generic output item is not simply replaced by another string,
				but each time a generic output item is found, the
				CallbackRenderGenericOutputItem() callback function
				is invoked.
				However, we must tell the website what to look for.
				In addition generic output items support parameters.
			*/
			$this->RegisterGenericOutputItem('GOOGLEURL',array('search'));
			
			/*
				This will cause a log event each time a page is loaded.
				You may want to remove this if not needed.
			*/
			$this->SystemLog('INFORMATIONAL','REQUEST', $this->GetContent());


			$this->SetLayout('$$$websitename$$$');
			
			$this->LoadStringResources('systemadmin');
			$this->LoadStringResources('devtools');
			

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetPolicy($this->GetConfig('passwordpolicy'));

			$this->IncludeModule('user/signinonestep');
			$this->m_moduleSigninOneStep = new CSigninOneStepModule($this);
			$this->RegisterModule($this->m_moduleSigninOneStep);

			$this->IncludeModule('user/signin');
			$this->m_moduleSignin = new CSigninModule($this,'signin',$passwordpolicy,'signin','index'); 
			$this->RegisterModule($this->m_moduleSignin);

			$this->IncludeModule('user/signout');
			$this->m_moduleSignOut = new CSignOutModule($this,'test-signout'); 
			$this->RegisterModule($this->m_moduleSignOut);

			$this->IncludeModule('user/login');
			$this->m_moduleLogin = new CLoginModule($this,'login','logout');
			$this->RegisterModule($this->m_moduleLogin);
			
			$this->IncludeModule('user/useremail');
			$this->m_moduleUserEmail = new CUserEmailModule($this,true,true,'verifymail',60*60);
			$this->RegisterModule($this->m_moduleUserEmail);

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
	
	
		function CallbackUserCheckName($strUsername)
		{
			if (parent::CallbackUserCheckName($strUsername) == false)
			{
				return false;	
			}
			/*
				This makes sure all new user names consist of lower
				case characters or numbers.
				Remove this if you want to allow more options!
				If for example the website should only accept valid
				email addresses as usernames, this would be the place
				to enforce this.
			*/
			if (StringCheckCharSet($strUsername,CHARSET_ALPHANUMERIC_LOWERCASE) == false)
			{
				return false;	
			}
			return true;
		}
		
		function CallbackUserChangePassword($strOldPassword,$strNewPassword)
		{
			$bResult = parent::CallbackUserChangePassword($strOldPassword,$strNewPassword);
			if (!$bResult)
			{
				return false;
			}
			
			$strUserID = $this->GetAuthUserID();
			if ($strUserID == '')
			{
				return false;	
			}

			return true;
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
			
			// DATETIME
			$this->IncludeElement('datetime');
			$datetime = new CDateTimeElement($this,'default');
			$this->SetOutputItem('DATETIME',$datetime->RenderDateTime(GetTimeNow()));	

			// USER STATUS
			$this->SetOutputItem('USERSTATUS',$this->m_moduleLogin->RenderModule('status'));

			// LANGUAGE SELECTION
			$this->IncludeModule('user/languageselection');
			$languageselectionModule = new CLanguageSelectionModule($this,'index');
			$this->SetOutputItem('LANGUAGESELECTION',$languageselectionModule->RenderModule('toolbar'));	
		}
		
		function CallbackRenderSpecialContent($strContentID)
		{
			/*
				Before trying to load the content from a file, this function is called.
				If the function returns '', the content is loaded from a file.
				If the function returns anything else, the returned string is used as the content.
			*/
			
			/*
				The following is required for the signin process.
				A user has entered a user name, password, and mail address.
				Then a mail has been sent to the user with a confirmation link.
				This link includes the 'signin' parameter which is handled here.
				The actual logic takes place in the user/signin module.
			*/
			if ($this->GetVar('signin') != '')
			{
				$this->SetVar('disclaimerconfirmed','1');
				$strOutput = $this->m_moduleSignin->RenderModule('default');
				$strOutput .= $this->m_moduleLogin->RenderModule('form');
				return $strOutput;
			}		
			
			/*
				The following code manages a user's request to change the
				stored email address.
				After the user has changed the email address in the user
				settings page in the module user/useremail a mail has been
				sent to the user asking for confirmation.
				This mail contains a confirmation link that includes the
				'verifymail' parameter.
				The actual logic is in the module user/useremail.
			*/			
			if ($this->GetVar('verifymail') != '')
			{
				$this->SetVar('disclaimerconfirmed','1');
				$this->m_moduleUserEmail->SetState('validation');
				$strOutput = $this->m_moduleUserEmail->RenderModule();
				return $strOutput;
			}			
			
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

		function CallbackRenderContentException()
		{
			/*
				This function is called if a requested content is not found.
				If you were implementing a wiki, this would be the place to create a
				new page.
			*/
			return parent::CallbackRenderContentException();
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
				The parent implementation in CWebsite::CallbackCheckCondition() always returns
				false.
				CUsersWebSite::CallbackCheckCondition() on the other hand will check
				the users individual privileges.
				You can also use this function to enable or disable features globally or
				apply additional checks before granting a user access to specific features.
			*/
			
			

			/*
				This logic here will grant the 'useradmin' privilege to those
				users who have the 'systemadmin' privilege.
				This makes the 'useradmin' privilege a part of the 'systemadmin' privilege.
			*/
			if ($strCondition == 'useradmin')
			{
				if ($this->CheckCondition('systemadmin') == true)
				{
					return true;
				}
			}


			/*
				This logic here will grant the 'admin' privilege to those
				users who have the 'systemadmin' or 'useradmin' privilege.
				This is to make the 'admin' content visible to all users who have
				any admin-related privilege.
			*/
			if ($strCondition == 'admin')
			{
				if ($this->CheckCondition('useradmin') == true)
				{
					return true;
				}
				if ($this->CheckCondition('systemadmin') == true)
				{
					return true;
				}
				/*
					Add your own administration specific privileges here...
				*/
			}			
			
			/*
				This will enable the Developer Tools section to everyone.
			*/
			if ($strCondition == 'devtools')
			{
				return true;	
			}
			
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
				same as CWebsite::RenderErrorImage() so this is just for demonstration purposes.
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
	

