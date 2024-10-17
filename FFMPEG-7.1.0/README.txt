FFmpeg 64-bit shared Windows build from www.gyan.dev

Version: 7.1-full_build-www.gyan.dev

License: GPL v3

Source Code: https://github.com/FFmpeg/FFmpeg/commit/b08d7969c5

External Assets
frei0r plugins:   https://www.gyan.dev/ffmpeg/builds/ffmpeg-frei0r-plugins
lensfun database: https://www.gyan.dev/ffmpeg/builds/ffmpeg-lensfun-db

release-full build configuration: 

ARCH                      x86 (generic)
big-endian                no
runtime cpu detection     yes
standalone assembly       yes
x86 assembler             nasm
MMX enabled               yes
MMXEXT enabled            yes
3DNow! enabled            yes
3DNow! extended enabled   yes
SSE enabled               yes
SSSE3 enabled             yes
AESNI enabled             yes
AVX enabled               yes
AVX2 enabled              yes
AVX-512 enabled           yes
AVX-512ICL enabled        yes
XOP enabled               yes
FMA3 enabled              yes
FMA4 enabled              yes
i686 features enabled     yes
CMOV is fast              yes
EBX available             yes
EBP available             yes
debug symbols             yes
strip symbols             yes
optimize for size         no
optimizations             yes
static                    no
shared                    yes
postprocessing support    yes
network support           yes
threading support         pthreads
safe bitstream reader     yes
texi2html enabled         no
perl enabled              yes
pod2man enabled           yes
makeinfo enabled          yes
makeinfo supports HTML    yes
xmllint enabled           yes

External libraries:
avisynth                libilbc                 libtheora
bzlib                   libjxl                  libtwolame
chromaprint             liblc3                  libuavs3d
frei0r                  liblensfun              libvidstab
gmp                     libmodplug              libvmaf
gnutls                  libmp3lame              libvo_amrwbenc
iconv                   libmysofa               libvorbis
ladspa                  libopencore_amrnb       libvpx
libaom                  libopencore_amrwb       libvvenc
libaribb24              libopenjpeg             libwebp
libaribcaption          libopenmpt              libx264
libass                  libopus                 libx265
libbluray               libplacebo              libxavs2
libbs2b                 libqrencode             libxevd
libcaca                 libquirc                libxeve
libcdio                 librav1e                libxml2
libcodec2               librist                 libxvid
libdav1d                librubberband           libzimg
libdavs2                libshaderc              libzmq
libflite                libshine                libzvbi
libfontconfig           libsnappy               lzma
libfreetype             libsoxr                 mediafoundation
libfribidi              libspeex                sdl2
libgme                  libsrt                  zlib
libgsm                  libssh
libharfbuzz             libsvtav1

External libraries providing hardware acceleration:
amf                     d3d12va                 nvdec
cuda                    dxva2                   nvenc
cuda_llvm               ffnvcodec               opencl
cuvid                   libmfx                  vaapi
d3d11va                 libvpl                  vulkan

Libraries:
avcodec                 avformat                swresample
avdevice                avutil                  swscale
avfilter                postproc

Programs:
ffmpeg                  ffplay                  ffprobe

Enabled decoders:
aac                     g723_1                  pcx
aac_fixed               g729                    pdv
aac_latm                gdv                     pfm
aasc                    gem                     pgm
ac3                     gif                     pgmyuv
ac3_fixed               gremlin_dpcm            pgssub
acelp_kelvin            gsm                     pgx
adpcm_4xm               gsm_ms                  phm
adpcm_adx               h261                    photocd
adpcm_afc               h263                    pictor
adpcm_agm               h263i                   pixlet
adpcm_aica              h263p                   pjs
adpcm_argo              h264                    png
adpcm_ct                h264_cuvid              ppm
adpcm_dtk               h264_qsv                prores
adpcm_ea                hap                     prosumer
adpcm_ea_maxis_xa       hca                     psd
adpcm_ea_r1             hcom                    ptx
adpcm_ea_r2             hdr                     qcelp
adpcm_ea_r3             hevc                    qdm2
adpcm_ea_xas            hevc_cuvid              qdmc
adpcm_g722              hevc_qsv                qdraw
adpcm_g726              hnm4_video              qoa
adpcm_g726le            hq_hqa                  qoi
adpcm_ima_acorn         hqx                     qpeg
adpcm_ima_alp           huffyuv                 qtrle
adpcm_ima_amv           hymt                    r10k
adpcm_ima_apc           iac                     r210
adpcm_ima_apm           idcin                   ra_144
adpcm_ima_cunning       idf                     ra_288
adpcm_ima_dat4          iff_ilbm                ralf
adpcm_ima_dk3           ilbc                    rasc
adpcm_ima_dk4           imc                     rawvideo
adpcm_ima_ea_eacs       imm4                    realtext
adpcm_ima_ea_sead       imm5                    rka
adpcm_ima_iss           indeo2                  rl2
adpcm_ima_moflex        indeo3                  roq
adpcm_ima_mtf           indeo4                  roq_dpcm
adpcm_ima_oki           indeo5                  rpza
adpcm_ima_qt            interplay_acm           rscc
adpcm_ima_rad           interplay_dpcm          rtv1
adpcm_ima_smjpeg        interplay_video         rv10
adpcm_ima_ssi           ipu                     rv20
adpcm_ima_wav           jacosub                 rv30
adpcm_ima_ws            jpeg2000                rv40
adpcm_ms                jpegls                  s302m
adpcm_mtaf              jv                      sami
adpcm_psx               kgv1                    sanm
adpcm_sbpro_2           kmvc                    sbc
adpcm_sbpro_3           lagarith                scpr
adpcm_sbpro_4           lead                    screenpresso
adpcm_swf               libaom_av1              sdx2_dpcm
adpcm_thp               libaribb24              sga
adpcm_thp_le            libaribcaption          sgi
adpcm_vima              libcodec2               sgirle
adpcm_xa                libdav1d                sheervideo
adpcm_xmd               libdavs2                shorten
adpcm_yamaha            libgsm                  simbiosis_imx
adpcm_zork              libgsm_ms               sipr
agm                     libilbc                 siren
aic                     libjxl                  smackaud
alac                    liblc3                  smacker
alias_pix               libopencore_amrnb       smc
als                     libopencore_amrwb       smvjpeg
amrnb                   libopus                 snow
amrwb                   libspeex                sol_dpcm
amv                     libuavs3d               sonic
anm                     libvorbis               sp5x
ansi                    libvpx_vp8              speedhq
anull                   libvpx_vp9              speex
apac                    libxevd                 srgc
ape                     libzvbi_teletext        srt
apng                    loco                    ssa
aptx                    lscr                    stl
aptx_hd                 m101                    subrip
arbc                    mace3                   subviewer
argo                    mace6                   subviewer1
ass                     magicyuv                sunrast
asv1                    mdec                    svq1
asv2                    media100                svq3
atrac1                  metasound               tak
atrac3                  microdvd                targa
atrac3al                mimic                   targa_y216
atrac3p                 misc4                   tdsc
atrac3pal               mjpeg                   text
atrac9                  mjpeg_cuvid             theora
aura                    mjpeg_qsv               thp
aura2                   mjpegb                  tiertexseqvideo
av1                     mlp                     tiff
av1_cuvid               mmvideo                 tmv
av1_qsv                 mobiclip                truehd
avrn                    motionpixels            truemotion1
avrp                    movtext                 truemotion2
avs                     mp1                     truemotion2rt
avui                    mp1float                truespeech
bethsoftvid             mp2                     tscc
bfi                     mp2float                tscc2
bink                    mp3                     tta
binkaudio_dct           mp3adu                  twinvq
binkaudio_rdft          mp3adufloat             txd
bintext                 mp3float                ulti
bitpacked               mp3on4                  utvideo
bmp                     mp3on4float             v210
bmv_audio               mpc7                    v210x
bmv_video               mpc8                    v308
bonk                    mpeg1_cuvid             v408
brender_pix             mpeg1video              v410
c93                     mpeg2_cuvid             vb
cavs                    mpeg2_qsv               vble
cbd2_dpcm               mpeg2video              vbn
ccaption                mpeg4                   vc1
cdgraphics              mpeg4_cuvid             vc1_cuvid
cdtoons                 mpegvideo               vc1_qsv
cdxl                    mpl2                    vc1image
cfhd                    msa1                    vcr1
cinepak                 mscc                    vmdaudio
clearvideo              msmpeg4v1               vmdvideo
cljr                    msmpeg4v2               vmix
cllc                    msmpeg4v3               vmnc
comfortnoise            msnsiren                vnull
cook                    msp2                    vorbis
cpia                    msrle                   vp3
cri                     mss1                    vp4
cscd                    mss2                    vp5
cyuv                    msvideo1                vp6
dca                     mszh                    vp6a
dds                     mts2                    vp6f
derf_dpcm               mv30                    vp7
dfa                     mvc1                    vp8
dfpwm                   mvc2                    vp8_cuvid
dirac                   mvdv                    vp8_qsv
dnxhd                   mvha                    vp9
dolby_e                 mwsc                    vp9_cuvid
dpx                     mxpeg                   vp9_qsv
dsd_lsbf                nellymoser              vplayer
dsd_lsbf_planar         notchlc                 vqa
dsd_msbf                nuv                     vqc
dsd_msbf_planar         on2avc                  vvc
dsicinaudio             opus                    vvc_qsv
dsicinvideo             osq                     wady_dpcm
dss_sp                  paf_audio               wavarc
dst                     paf_video               wavpack
dvaudio                 pam                     wbmp
dvbsub                  pbm                     wcmv
dvdsub                  pcm_alaw                webp
dvvideo                 pcm_bluray              webvtt
dxa                     pcm_dvd                 wmalossless
dxtory                  pcm_f16le               wmapro
dxv                     pcm_f24le               wmav1
eac3                    pcm_f32be               wmav2
eacmv                   pcm_f32le               wmavoice
eamad                   pcm_f64be               wmv1
eatgq                   pcm_f64le               wmv2
eatgv                   pcm_lxf                 wmv3
eatqi                   pcm_mulaw               wmv3image
eightbps                pcm_s16be               wnv1
eightsvx_exp            pcm_s16be_planar        wrapped_avframe
eightsvx_fib            pcm_s16le               ws_snd1
escape124               pcm_s16le_planar        xan_dpcm
escape130               pcm_s24be               xan_wc3
evrc                    pcm_s24daud             xan_wc4
exr                     pcm_s24le               xbin
fastaudio               pcm_s24le_planar        xbm
ffv1                    pcm_s32be               xface
ffvhuff                 pcm_s32le               xl
ffwavesynth             pcm_s32le_planar        xma1
fic                     pcm_s64be               xma2
fits                    pcm_s64le               xpm
flac                    pcm_s8                  xsub
flashsv                 pcm_s8_planar           xwd
flashsv2                pcm_sga                 y41p
flic                    pcm_u16be               ylc
flv                     pcm_u16le               yop
fmvc                    pcm_u24be               yuv4
fourxm                  pcm_u24le               zero12v
fraps                   pcm_u32be               zerocodec
frwu                    pcm_u32le               zlib
ftr                     pcm_u8                  zmbv
g2m                     pcm_vidc

Enabled encoders:
a64multi                hevc_vaapi              pcm_s8
a64multi5               hevc_vulkan             pcm_s8_planar
aac                     huffyuv                 pcm_u16be
aac_mf                  jpeg2000                pcm_u16le
ac3                     jpegls                  pcm_u24be
ac3_fixed               libaom_av1              pcm_u24le
ac3_mf                  libcodec2               pcm_u32be
adpcm_adx               libgsm                  pcm_u32le
adpcm_argo              libgsm_ms               pcm_u8
adpcm_g722              libilbc                 pcm_vidc
adpcm_g726              libjxl                  pcx
adpcm_g726le            liblc3                  pfm
adpcm_ima_alp           libmp3lame              pgm
adpcm_ima_amv           libopencore_amrnb       pgmyuv
adpcm_ima_apm           libopenjpeg             phm
adpcm_ima_qt            libopus                 png
adpcm_ima_ssi           librav1e                ppm
adpcm_ima_wav           libshine                prores
adpcm_ima_ws            libspeex                prores_aw
adpcm_ms                libsvtav1               prores_ks
adpcm_swf               libtheora               qoi
adpcm_yamaha            libtwolame              qtrle
alac                    libvo_amrwbenc          r10k
alias_pix               libvorbis               r210
amv                     libvpx_vp8              ra_144
anull                   libvpx_vp9              rawvideo
apng                    libvvenc                roq
aptx                    libwebp                 roq_dpcm
aptx_hd                 libwebp_anim            rpza
ass                     libx264                 rv10
asv1                    libx264rgb              rv20
asv2                    libx265                 s302m
av1_amf                 libxavs2                sbc
av1_nvenc               libxeve                 sgi
av1_qsv                 libxvid                 smc
av1_vaapi               ljpeg                   snow
avrp                    magicyuv                sonic
avui                    mjpeg                   sonic_ls
bitpacked               mjpeg_qsv               speedhq
bmp                     mjpeg_vaapi             srt
cfhd                    mlp                     ssa
cinepak                 movtext                 subrip
cljr                    mp2                     sunrast
comfortnoise            mp2fixed                svq1
dca                     mp3_mf                  targa
dfpwm                   mpeg1video              text
dnxhd                   mpeg2_qsv               tiff
dpx                     mpeg2_vaapi             truehd
dvbsub                  mpeg2video              tta
dvdsub                  mpeg4                   ttml
dvvideo                 msmpeg4v2               utvideo
dxv                     msmpeg4v3               v210
eac3                    msrle                   v308
exr                     msvideo1                v408
ffv1                    nellymoser              v410
ffvhuff                 opus                    vbn
fits                    pam                     vc2
flac                    pbm                     vnull
flashsv                 pcm_alaw                vorbis
flashsv2                pcm_bluray              vp8_vaapi
flv                     pcm_dvd                 vp9_qsv
g723_1                  pcm_f32be               vp9_vaapi
gif                     pcm_f32le               wavpack
h261                    pcm_f64be               wbmp
h263                    pcm_f64le               webvtt
h263p                   pcm_mulaw               wmav1
h264_amf                pcm_s16be               wmav2
h264_mf                 pcm_s16be_planar        wmv1
h264_nvenc              pcm_s16le               wmv2
h264_qsv                pcm_s16le_planar        wrapped_avframe
h264_vaapi              pcm_s24be               xbm
h264_vulkan             pcm_s24daud             xface
hap                     pcm_s24le               xsub
hdr                     pcm_s24le_planar        xwd
hevc_amf                pcm_s32be               y41p
hevc_d3d12va            pcm_s32le               yuv4
hevc_mf                 pcm_s32le_planar        zlib
hevc_nvenc              pcm_s64be               zmbv
hevc_qsv                pcm_s64le

Enabled hwaccels:
av1_d3d11va             hevc_dxva2              vc1_dxva2
av1_d3d11va2            hevc_nvdec              vc1_nvdec
av1_d3d12va             hevc_vaapi              vc1_vaapi
av1_dxva2               hevc_vulkan             vp8_nvdec
av1_nvdec               mjpeg_nvdec             vp8_vaapi
av1_vaapi               mjpeg_vaapi             vp9_d3d11va
av1_vulkan              mpeg1_nvdec             vp9_d3d11va2
h263_vaapi              mpeg2_d3d11va           vp9_d3d12va
h264_d3d11va            mpeg2_d3d11va2          vp9_dxva2
h264_d3d11va2           mpeg2_d3d12va           vp9_nvdec
h264_d3d12va            mpeg2_dxva2             vp9_vaapi
h264_dxva2              mpeg2_nvdec             wmv3_d3d11va
h264_nvdec              mpeg2_vaapi             wmv3_d3d11va2
h264_vaapi              mpeg4_nvdec             wmv3_d3d12va
h264_vulkan             mpeg4_vaapi             wmv3_dxva2
hevc_d3d11va            vc1_d3d11va             wmv3_nvdec
hevc_d3d11va2           vc1_d3d11va2            wmv3_vaapi
hevc_d3d12va            vc1_d3d12va

Enabled parsers:
aac                     dvdsub                  mpegaudio
aac_latm                evc                     mpegvideo
ac3                     flac                    opus
adx                     ftr                     png
amr                     g723_1                  pnm
av1                     g729                    qoi
avs2                    gif                     rv34
avs3                    gsm                     sbc
bmp                     h261                    sipr
cavsvideo               h263                    tak
cook                    h264                    vc1
cri                     hdr                     vorbis
dca                     hevc                    vp3
dirac                   ipu                     vp8
dnxhd                   jpeg2000                vp9
dolby_e                 jpegxl                  vvc
dpx                     misc4                   webp
dvaudio                 mjpeg                   xbm
dvbsub                  mlp                     xma
dvd_nav                 mpeg4video              xwd

Enabled demuxers:
aa                      idf                     pcm_f64le
aac                     iff                     pcm_mulaw
aax                     ifv                     pcm_s16be
ac3                     ilbc                    pcm_s16le
ac4                     image2                  pcm_s24be
ace                     image2_alias_pix        pcm_s24le
acm                     image2_brender_pix      pcm_s32be
act                     image2pipe              pcm_s32le
adf                     image_bmp_pipe          pcm_s8
adp                     image_cri_pipe          pcm_u16be
ads                     image_dds_pipe          pcm_u16le
adx                     image_dpx_pipe          pcm_u24be
aea                     image_exr_pipe          pcm_u24le
afc                     image_gem_pipe          pcm_u32be
aiff                    image_gif_pipe          pcm_u32le
aix                     image_hdr_pipe          pcm_u8
alp                     image_j2k_pipe          pcm_vidc
amr                     image_jpeg_pipe         pdv
amrnb                   image_jpegls_pipe       pjs
amrwb                   image_jpegxl_pipe       pmp
anm                     image_pam_pipe          pp_bnk
apac                    image_pbm_pipe          pva
apc                     image_pcx_pipe          pvf
ape                     image_pfm_pipe          qcp
apm                     image_pgm_pipe          qoa
apng                    image_pgmyuv_pipe       r3d
aptx                    image_pgx_pipe          rawvideo
aptx_hd                 image_phm_pipe          rcwt
aqtitle                 image_photocd_pipe      realtext
argo_asf                image_pictor_pipe       redspark
argo_brp                image_png_pipe          rka
argo_cvg                image_ppm_pipe          rl2
asf                     image_psd_pipe          rm
asf_o                   image_qdraw_pipe        roq
ass                     image_qoi_pipe          rpl
ast                     image_sgi_pipe          rsd
au                      image_sunrast_pipe      rso
av1                     image_svg_pipe          rtp
avi                     image_tiff_pipe         rtsp
avisynth                image_vbn_pipe          s337m
avr                     image_webp_pipe         sami
avs                     image_xbm_pipe          sap
avs2                    image_xpm_pipe          sbc
avs3                    image_xwd_pipe          sbg
bethsoftvid             imf                     scc
bfi                     ingenient               scd
bfstm                   ipmovie                 sdns
bink                    ipu                     sdp
binka                   ircam                   sdr2
bintext                 iss                     sds
bit                     iv8                     sdx
bitpacked               ivf                     segafilm
bmv                     ivr                     ser
boa                     jacosub                 sga
bonk                    jpegxl_anim             shorten
brstm                   jv                      siff
c93                     kux                     simbiosis_imx
caf                     kvag                    sln
cavsvideo               laf                     smacker
cdg                     lc3                     smjpeg
cdxl                    libgme                  smush
cine                    libmodplug              sol
codec2                  libopenmpt              sox
codec2raw               live_flv                spdif
concat                  lmlm4                   srt
dash                    loas                    stl
data                    lrc                     str
daud                    luodat                  subviewer
dcstr                   lvf                     subviewer1
derf                    lxf                     sup
dfa                     m4v                     svag
dfpwm                   matroska                svs
dhav                    mca                     swf
dirac                   mcc                     tak
dnxhd                   mgsts                   tedcaptions
dsf                     microdvd                thp
dsicin                  mjpeg                   threedostr
dss                     mjpeg_2000              tiertexseq
dts                     mlp                     tmv
dtshd                   mlv                     truehd
dv                      mm                      tta
dvbsub                  mmf                     tty
dvbtxt                  mods                    txd
dxa                     moflex                  ty
ea                      mov                     usm
ea_cdata                mp3                     v210
eac3                    mpc                     v210x
epaf                    mpc8                    vag
evc                     mpegps                  vc1
ffmetadata              mpegts                  vc1t
filmstrip               mpegtsraw               vividas
fits                    mpegvideo               vivo
flac                    mpjpeg                  vmd
flic                    mpl2                    vobsub
flv                     mpsub                   voc
fourxm                  msf                     vpk
frm                     msnwc_tcp               vplayer
fsb                     msp                     vqf
fwse                    mtaf                    vvc
g722                    mtv                     w64
g723_1                  musx                    wady
g726                    mv                      wav
g726le                  mvi                     wavarc
g729                    mxf                     wc3
gdv                     mxg                     webm_dash_manifest
genh                    nc                      webvtt
gif                     nistsphere              wsaud
gsm                     nsp                     wsd
gxf                     nsv                     wsvqa
h261                    nut                     wtv
h263                    nuv                     wv
h264                    obu                     wve
hca                     ogg                     xa
hcom                    oma                     xbin
hevc                    osq                     xmd
hls                     paf                     xmv
hnm                     pcm_alaw                xvag
iamf                    pcm_f32be               xwma
ico                     pcm_f32le               yop
idcin                   pcm_f64be               yuv4mpegpipe

Enabled muxers:
a64                     h263                    pcm_s24be
ac3                     h264                    pcm_s24le
ac4                     hash                    pcm_s32be
adts                    hds                     pcm_s32le
adx                     hevc                    pcm_s8
aea                     hls                     pcm_u16be
aiff                    iamf                    pcm_u16le
alp                     ico                     pcm_u24be
amr                     ilbc                    pcm_u24le
amv                     image2                  pcm_u32be
apm                     image2pipe              pcm_u32le
apng                    ipod                    pcm_u8
aptx                    ircam                   pcm_vidc
aptx_hd                 ismv                    psp
argo_asf                ivf                     rawvideo
argo_cvg                jacosub                 rcwt
asf                     kvag                    rm
asf_stream              latm                    roq
ass                     lc3                     rso
ast                     lrc                     rtp
au                      m4v                     rtp_mpegts
avi                     matroska                rtsp
avif                    matroska_audio          sap
avm2                    md5                     sbc
avs2                    microdvd                scc
avs3                    mjpeg                   segafilm
bit                     mkvtimestamp_v2         segment
caf                     mlp                     smjpeg
cavsvideo               mmf                     smoothstreaming
chromaprint             mov                     sox
codec2                  mp2                     spdif
codec2raw               mp3                     spx
crc                     mp4                     srt
dash                    mpeg1system             stream_segment
data                    mpeg1vcd                streamhash
daud                    mpeg1video              sup
dfpwm                   mpeg2dvd                swf
dirac                   mpeg2svcd               tee
dnxhd                   mpeg2video              tg2
dts                     mpeg2vob                tgp
dv                      mpegts                  truehd
eac3                    mpjpeg                  tta
evc                     mxf                     ttml
f4v                     mxf_d10                 uncodedframecrc
ffmetadata              mxf_opatom              vc1
fifo                    null                    vc1t
filmstrip               nut                     voc
fits                    obu                     vvc
flac                    oga                     w64
flv                     ogg                     wav
framecrc                ogv                     webm
framehash               oma                     webm_chunk
framemd5                opus                    webm_dash_manifest
g722                    pcm_alaw                webp
g723_1                  pcm_f32be               webvtt
g726                    pcm_f32le               wsaud
g726le                  pcm_f64be               wtv
gif                     pcm_f64le               wv
gsm                     pcm_mulaw               yuv4mpegpipe
gxf                     pcm_s16be
h261                    pcm_s16le

Enabled protocols:
async                   http                    rtmp
bluray                  httpproxy               rtmpe
cache                   https                   rtmps
concat                  icecast                 rtmpt
concatf                 ipfs_gateway            rtmpte
crypto                  ipns_gateway            rtmpts
data                    librist                 rtp
fd                      libsrt                  srtp
ffrtmpcrypt             libssh                  subfile
ffrtmphttp              libzmq                  tcp
file                    md5                     tee
ftp                     mmsh                    tls
gopher                  mmst                    udp
gophers                 pipe                    udplite
hls                     prompeg

Enabled filters:
a3dscope                ddagrab                 pan
aap                     deband                  perlin
abench                  deblock                 perms
abitscope               decimate                perspective
acompressor             deconvolve              phase
acontrast               dedot                   photosensitivity
acopy                   deesser                 pixdesctest
acrossfade              deflate                 pixelize
acrossover              deflicker               pixscope
acrusher                deinterlace_qsv         pp
acue                    deinterlace_vaapi       pp7
addroi                  dejudder                premultiply
adeclick                delogo                  prewitt
adeclip                 denoise_vaapi           prewitt_opencl
adecorrelate            deshake                 procamp_vaapi
adelay                  deshake_opencl          program_opencl
adenorm                 despill                 pseudocolor
aderivative             detelecine              psnr
adrawgraph              dialoguenhance          pullup
adrc                    dilation                qp
adynamicequalizer       dilation_opencl         qrencode
adynamicsmooth          displace                qrencodesrc
aecho                   doubleweave             quirc
aemphasis               drawbox                 random
aeval                   drawbox_vaapi           readeia608
aevalsrc                drawgraph               readvitc
aexciter                drawgrid                realtime
afade                   drawtext                remap
afdelaysrc              drmeter                 remap_opencl
afftdn                  dynaudnorm              removegrain
afftfilt                earwax                  removelogo
afir                    ebur128                 repeatfields
afireqsrc               edgedetect              replaygain
afirsrc                 elbg                    reverse
aformat                 entropy                 rgbashift
afreqshift              epx                     rgbtestsrc
afwtdn                  eq                      roberts
agate                   equalizer               roberts_opencl
agraphmonitor           erosion                 rotate
ahistogram              erosion_opencl          rubberband
aiir                    estdif                  sab
aintegral               exposure                scale
ainterleave             extractplanes           scale2ref
alatency                extrastereo             scale_cuda
alimiter                fade                    scale_qsv
allpass                 feedback                scale_vaapi
allrgb                  fftdnoiz                scale_vulkan
allyuv                  fftfilt                 scdet
aloop                   field                   scharr
alphaextract            fieldhint               scroll
alphamerge              fieldmatch              segment
amerge                  fieldorder              select
ametadata               fillborders             selectivecolor
amix                    find_rect               sendcmd
amovie                  firequalizer            separatefields
amplify                 flanger                 setdar
amultiply               flip_vulkan             setfield
anequalizer             flite                   setparams
anlmdn                  floodfill               setpts
anlmf                   format                  setrange
anlms                   fps                     setsar
anoisesrc               framepack               settb
anull                   framerate               sharpness_vaapi
anullsink               framestep               shear
anullsrc                freezedetect            showcqt
apad                    freezeframes            showcwt
aperms                  frei0r                  showfreqs
aphasemeter             frei0r_src              showinfo
aphaser                 fspp                    showpalette
aphaseshift             fsync                   showspatial
apsnr                   gblur                   showspectrum
apsyclip                gblur_vulkan            showspectrumpic
apulsator               geq                     showvolume
arealtime               gradfun                 showwaves
aresample               gradients               showwavespic
areverse                graphmonitor            shuffleframes
arls                    grayworld               shufflepixels
arnndn                  greyedge                shuffleplanes
asdr                    guided                  sidechaincompress
asegment                haas                    sidechaingate
aselect                 haldclut                sidedata
asendcmd                haldclutsrc             sierpinski
asetnsamples            hdcd                    signalstats
asetpts                 headphone               signature
asetrate                hflip                   silencedetect
asettb                  hflip_vulkan            silenceremove
ashowinfo               highpass                sinc
asidedata               highshelf               sine
asisdr                  hilbert                 siti
asoftclip               histeq                  smartblur
aspectralstats          histogram               smptebars
asplit                  hqdn3d                  smptehdbars
ass                     hqx                     sobel
astats                  hstack                  sobel_opencl
astreamselect           hstack_qsv              sofalizer
asubboost               hstack_vaapi            spectrumsynth
asubcut                 hsvhold                 speechnorm
asupercut               hsvkey                  split
asuperpass              hue                     spp
asuperstop              huesaturation           ssim
atadenoise              hwdownload              ssim360
atempo                  hwmap                   stereo3d
atilt                   hwupload                stereotools
atrim                   hwupload_cuda           stereowiden
avectorscope            hysteresis              streamselect
avgblur                 identity                subtitles
avgblur_opencl          idet                    super2xsai
avgblur_vulkan          il                      superequalizer
avsynctest              inflate                 surround
axcorrelate             interlace               swaprect
azmq                    interleave              swapuv
backgroundkey           join                    tblend
bandpass                kerndeint               telecine
bandreject              kirsch                  testsrc
bass                    ladspa                  testsrc2
bbox                    lagfun                  thistogram
bench                   latency                 threshold
bilateral               lenscorrection          thumbnail
bilateral_cuda          lensfun                 thumbnail_cuda
biquad                  libplacebo              tile
bitplanenoise           libvmaf                 tiltandshift
blackdetect             life                    tiltshelf
blackframe              limitdiff               tinterlace
blend                   limiter                 tlut2
blend_vulkan            loop                    tmedian
blockdetect             loudnorm                tmidequalizer
blurdetect              lowpass                 tmix
bm3d                    lowshelf                tonemap
boxblur                 lumakey                 tonemap_opencl
boxblur_opencl          lut                     tonemap_vaapi
bs2b                    lut1d                   tpad
bwdif                   lut2                    transpose
bwdif_cuda              lut3d                   transpose_opencl
bwdif_vulkan            lutrgb                  transpose_vaapi
cas                     lutyuv                  transpose_vulkan
ccrepack                mandelbrot              treble
cellauto                maskedclamp             tremolo
channelmap              maskedmax               trim
channelsplit            maskedmerge             unpremultiply
chorus                  maskedmin               unsharp
chromaber_vulkan        maskedthreshold         unsharp_opencl
chromahold              maskfun                 untile
chromakey               mcdeint                 uspp
chromakey_cuda          mcompand                v360
chromanr                median                  vaguedenoiser
chromashift             mergeplanes             varblur
ciescope                mestimate               vectorscope
codecview               metadata                vflip
color                   midequalizer            vflip_vulkan
color_vulkan            minterpolate            vfrdet
colorbalance            mix                     vibrance
colorchannelmixer       monochrome              vibrato
colorchart              morpho                  vidstabdetect
colorcontrast           movie                   vidstabtransform
colorcorrect            mpdecimate              vif
colorhold               mptestsrc               vignette
colorize                msad                    virtualbass
colorkey                multiply                vmafmotion
colorkey_opencl         negate                  volume
colorlevels             nlmeans                 volumedetect
colormap                nlmeans_opencl          vpp_qsv
colormatrix             nlmeans_vulkan          vstack
colorspace              nnedi                   vstack_qsv
colorspace_cuda         noformat                vstack_vaapi
colorspectrum           noise                   w3fdif
colortemperature        normalize               waveform
compand                 null                    weave
compensationdelay       nullsink                xbr
concat                  nullsrc                 xcorrelate
convolution             openclsrc               xfade
convolution_opencl      oscilloscope            xfade_opencl
convolve                overlay                 xfade_vulkan
copy                    overlay_cuda            xmedian
corr                    overlay_opencl          xpsnr
cover_rect              overlay_qsv             xstack
crop                    overlay_vaapi           xstack_qsv
cropdetect              overlay_vulkan          xstack_vaapi
crossfeed               owdenoise               yadif
crystalizer             pad                     yadif_cuda
cue                     pad_opencl              yaepblur
curves                  pad_vaapi               yuvtestsrc
datascope               pal100bars              zmq
dblur                   pal75bars               zoneplate
dcshift                 palettegen              zoompan
dctdnoiz                paletteuse              zscale

Enabled bsfs:
aac_adtstoasc           h264_mp4toannexb        pcm_rechunk
av1_frame_merge         h264_redundant_pps      pgs_frame_merge
av1_frame_split         hapqa_extract           prores_metadata
av1_metadata            hevc_metadata           remove_extradata
chomp                   hevc_mp4toannexb        setts
dca_core                imx_dump_header         showinfo
dovi_rpu                media100_to_mjpegb      text2movsub
dts2pts                 mjpeg2jpeg              trace_headers
dump_extradata          mjpega_dump_header      truehd_core
dv_error_marker         mov2textsub             vp9_metadata
eac3_core               mpeg2_metadata          vp9_raw_reorder
evc_frame_merge         mpeg4_unpack_bframes    vp9_superframe
extract_extradata       noise                   vp9_superframe_split
filter_units            null                    vvc_metadata
h264_metadata           opus_metadata           vvc_mp4toannexb

Enabled indevs:
dshow                   lavfi                   vfwcap
gdigrab                 libcdio

Enabled outdevs:
caca                    sdl2

release-full external libraries' versions: 

AMF v1.4.34-6-g3db6164
aom v3.10.0-110-g3817481261
aribb24 v1.0.3-5-g5e9be27
aribcaption 1.1.1
AviSynthPlus v3.7.3-75-gc6649361
bs2b 3.1.0
chromaprint 1.5.1
codec2 1.2.0-103-gff00a6e2
dav1d 1.4.2-66-gf2c3ccd
davs2 1.7-1-gb41cf11
ffnvcodec n12.2.72.0-1-g9934f17
flite v2.2-55-g6c9f20d
freetype VER-2-13-3
frei0r v2.3.3-5-gb528b26
fribidi v1.0.16
gsm 1.0.22
harfbuzz 10.0.1-1-gc7ef6a2e
ladspa-sdk 1.17
lame 3.100
lc3 1.1.0
lensfun v0.3.95-1540-g494c76b7
libass 0.17.3-32-g5298859
libcdio-paranoia 10.2
libgme 0.6.3
libilbc v3.0.4-346-g6adb26d4a4
libjxl v0.11-snapshot-25-g7e178a51
libopencore-amrnb 0.1.6
libopencore-amrwb 0.1.6
libplacebo v7.349.0-13-ga018ab04
libsoxr 0.1.3
libssh 0.11.1
libtheora 1.1.1
libwebp v1.4.0-97-g220ee529
oneVPL 2.13
OpenCL-Headers v2024.05.08-8-g542d7a8
openmpt libopenmpt-0.6.19-9-g1a29ac622
opus v1.5.2-21-gff6dea5e
qrencode 4.1.1
quirc 1.2
rav1e p20240924
rist 0.2.10
rubberband v1.8.1
SDL prerelease-2.29.2-377-g1edaad172
shaderc v2024.3-1-gd2564ba
shine 3.1.1
snappy 1.1.10
speex Speex-1.2.1-28-g1de1260
srt v1.5.4-rc.0-11-ga7b3711
SVT-AV1 v2.2.1-102-gaa853f1d
twolame 0.4.0
uavs3d v1.1-47-g1fd0491
VAAPI 2.23.0.
vidstab v1.1.1-13-g8dff7ad
vmaf v3.0.0-95-gd95b69e0
vo-amrwbenc 0.1.3
vorbis v1.3.7-10-g84c02369
vpx v1.14.1-371-g32de9c2be
vulkan-loader v1.3.296-2-g56dd9f2
vvenc v1.12.0-17-g0e7d4d3
x264 v0.164.3192
x265 4.0-6-ga009ec077
xavs2 1.4
xevd 0.5.0
xeve 0.5.1
xvid v1.3.7
zeromq 4.3.5
zimg release-3.0.5-150-g7143181
zvbi v0.2.42-58-ga48ab3a

