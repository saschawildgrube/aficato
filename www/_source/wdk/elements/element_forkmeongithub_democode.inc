$this->IncludeElement('forkmeongithub');
$forkmeongithub = new CForkMeOnGithubElement($this->GetWebsite(), 'default');

$strOutput = $forkmeongithub->RenderForkMeOnGithub('saschawildgrube','outbreak');