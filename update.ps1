param([string]$DestinationRoot = (Join-Path $PSScriptRoot '..'))

$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'

$repository = 'DLR-TS/SUMOLibraries'
$release = Invoke-RestMethod "https://api.github.com/repos/$repository/releases/latest" `
    -UseBasicParsing -TimeoutSec 30
$tag = [string]$release.tag_name
if ($tag -notmatch '\A[a-z0-9](?:[a-z0-9._-]*[a-z0-9])?\z') {
    throw "The latest release has an invalid tag: $tag"
}

$packageName = "SUMOLibraries-$tag"
$archiveName = "$packageName.zip"
$releaseUrl = "https://github.com/$repository/releases/download/$tag/$archiveName"
$assets = @($release.assets | Where-Object { $_.name -eq $archiveName })
if ($assets.Count -ne 1 -or $assets[0].browser_download_url -ne $releaseUrl -or $assets[0].size -le 0 -or
        $assets[0].digest -notmatch '\Asha256:([0-9a-f]{64})\z') {
    throw "The latest release does not list a valid $archiveName asset with a SHA-256 digest."
}
$expectedHash = $Matches[1]

$destination = Get-Item -LiteralPath $DestinationRoot
if ($destination -isnot [IO.DirectoryInfo]) {
    throw "The destination must be an existing directory: $DestinationRoot"
}
$target = Join-Path $destination.FullName $packageName
if (Test-Path -LiteralPath $target) {
    if (-not (Test-Path -LiteralPath $target -PathType Container)) {
        throw "A file occupies the release destination: $target"
    }
    Write-Host "Release $tag already has a destination at $target. The existing path was kept."
    return
}

$stageDirectory = [IO.DirectoryInfo](Join-Path $destination.FullName ('.sumolibs-' + [Guid]::NewGuid().ToString('N')))
if ($stageDirectory.Parent.FullName -ne $destination.FullName) {
    throw 'Cannot stage the archive inside the destination directory.'
}
$stage = $stageDirectory.FullName
New-Item -ItemType Directory -Path $stage | Out-Null
try {
    $archive = Join-Path $stage $archiveName
    Write-Host "Downloading $archiveName"
    Invoke-WebRequest $releaseUrl -UseBasicParsing -TimeoutSec 1800 -OutFile $archive
    if ((Get-Item -LiteralPath $archive).Length -ne $assets[0].size -or
            (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash -ne $expectedHash) {
        throw "The downloaded $archiveName failed release size or SHA-256 verification."
    }

    Write-Host "Extracting $archiveName"
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    $extracted = Join-Path $stage 'extracted'
    [IO.Compression.ZipFile]::ExtractToDirectory($archive, $extracted)
    $package = Join-Path $extracted $packageName
    if (-not (Test-Path -LiteralPath (Join-Path $package 'README.md') -PathType Leaf)) {
        throw "$archiveName does not contain the expected $packageName directory."
    }

    # Moving on the same volume keeps partial extractions out of the install path.
    [IO.Directory]::Move($package, $target)
    Write-Host "Installed SUMOLibraries $tag at $target"
} finally {
    Remove-Item -LiteralPath $stage -Recurse -Force
}
