/********************************************************************************
*                                                                               *
*                         OpenGL Application coding sample                      *
*                                                                               *
********************************************************************************/
#include "fx.h"
#include "fx3d.h"

#ifdef HAVE_GL_H

#define PRINT_YESNO(x) ( x ? "yes" : "no" )

// Timer setting (in nanoseconds)
const FXTime TIMER_INTERVAL=100000000;


/*******************************************************************************/


// Settings dialog thanks to Sander Jansen <sander@knology.net>
class GLSettingsDialog : public FXDialogBox {
  FXDECLARE(GLSettingsDialog)
private:
  FXGLCanvas *canvas;
  FXFont     *thickfont;
  FXList     *extensionlist;
protected:
  GLSettingsDialog();
  void setup();
private:
  GLSettingsDialog(const FXDialogBox&);
  GLSettingsDialog &operator=(const FXDialogBox&);
public:
  long onCmdSave(FXObject*,FXSelector,void*);
public:
  enum {
    ID_SAVE = FXDialogBox::ID_LAST
    };
public:
  GLSettingsDialog(FXApp * app,FXGLCanvas*ca);
  GLSettingsDialog(FXWindow* owner,FXGLCanvas*ca);
  FXbool saveInformation(const FXString & filename);
  virtual ~GLSettingsDialog();
  };


/*******************************************************************************/


// Implementation
FXDEFMAP(GLSettingsDialog) GLSettingsDialogMap[]={
  FXMAPFUNC(SEL_COMMAND,GLSettingsDialog::ID_SAVE,GLSettingsDialog::onCmdSave)
  };


FXIMPLEMENT(GLSettingsDialog,FXDialogBox,GLSettingsDialogMap,ARRAYNUMBER(GLSettingsDialogMap))


// Default constructor
GLSettingsDialog::GLSettingsDialog():canvas(nullptr),thickfont(nullptr),extensionlist(nullptr){
  }


// Construct with GL canvas (free floating version)
GLSettingsDialog::GLSettingsDialog(FXApp *a,FXGLCanvas *ca):FXDialogBox(a,FXString::null,DECOR_STRETCHABLE|DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0,0),canvas(ca),thickfont(nullptr),extensionlist(nullptr){
  setup();
  }


// Construct with GL canvas (modal version)
GLSettingsDialog::GLSettingsDialog(FXWindow *o,FXGLCanvas *ca):FXDialogBox(o,FXString::null,DECOR_STRETCHABLE|DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0,0),canvas(ca),thickfont(nullptr),extensionlist(nullptr){
  setup();
  }


void GLSettingsDialog::setup() {
  FXVerticalFrame   *vframe;
  FXHorizontalFrame *hframe;
  FXMatrix          *matrix;
  FXLabel           *label;

  FXFontDesc fontdescription=getApp()->getNormalFont()->getFontDesc();
  fontdescription.weight=FXFont::DemiBold;
  thickfont=new FXFont(getApp(),fontdescription);
  thickfont->create();

  FXGLVisual *glvisual=(FXGLVisual*)canvas->getVisual();
  canvas->makeCurrent();

  GLint	intval;
  GLint	intvals[2];
  char	*token, *text, *tmp;

  setTitle(tr("OpenGL Information"));

  hframe=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);
  new FXButton(hframe,tr("Close"),nullptr,this,ID_ACCEPT,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|BUTTON_DEFAULT|BUTTON_INITIAL,0,0,0,0,20,20,2,2);
  new FXButton(hframe,tr("Save"),nullptr,this,ID_SAVE,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT,0,0,0,0,20,20,2,2);

  FXTabBook *tabbook=new FXTabBook(this,nullptr,0,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);


  new FXTabItem(tabbook,tr(" Display Mode "));
  matrix=new FXMatrix(tabbook,2,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);

  label=new FXLabel(matrix,tr("Hardware-accelerated:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,tr(PRINT_YESNO(glvisual->isAccelerated())),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Double Buffering:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,tr(PRINT_YESNO(glvisual->isDoubleBuffer())),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Stereo View:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,tr(PRINT_YESNO(glvisual->isStereo())),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Buffer-swap by copy:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,tr(PRINT_YESNO(glvisual->isBufferSwapCopy())),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Color Depth:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%d",glvisual->getActualRedSize()+glvisual->getActualGreenSize()+glvisual->getActualBlueSize()+glvisual->getActualAlphaSize()),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Depth Buffer Size:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%d",glvisual->getActualDepthSize()),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Stencil Buffer Size:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%d",glvisual->getActualStencilSize()),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("RGBA:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%d-%d-%d-%d",glvisual->getActualRedSize(),glvisual->getActualGreenSize(),glvisual->getActualBlueSize(),glvisual->getActualAlphaSize()),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Accum RGBA:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%d-%d-%d-%d",glvisual->getActualAccumRedSize(),glvisual->getActualAccumGreenSize(),glvisual->getActualAccumBlueSize(),glvisual->getActualAccumAlphaSize()),nullptr,LABEL_NORMAL);

  label=new FXLabel(matrix,tr("Multi Sample:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%dx",glvisual->getActualMultiSamples()),nullptr,LABEL_NORMAL);




  new FXTabItem(tabbook,tr(" Limits "));
  matrix=new FXMatrix(tabbook, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);

  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, intvals);
  label=new FXLabel(matrix,tr("Viewport Size:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d x %d", intvals[0], intvals[1]));

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &intval);
  label=new FXLabel(matrix,tr("Texture Size:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));

#if defined(GL_MAX_TEXTURE_UNITS)
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &intval);
  label=new FXLabel(matrix,tr("Texture Units:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));
#endif

  glGetIntegerv(GL_MAX_LIGHTS, &intval);
  label=new FXLabel(matrix,tr("Lights:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));


  glGetIntegerv(GL_MAX_CLIP_PLANES, &intval);
  label=new FXLabel(matrix,tr("Clipping Planes:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));

  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &intval);
  label=new FXLabel(matrix,tr("Modelview Stack:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));

  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &intval);
  label=new FXLabel(matrix,tr("Projection Stack:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));

  glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &intval);
  label=new FXLabel(matrix,tr("Attribute Stack:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix, FXString::value("%d", intval));

  label=new FXLabel(matrix,tr("Vertex Attributes:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
#if defined(GL_MAX_VERTEX_ATTRIBS)
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &intval);
  new FXLabel(matrix, FXString::value("%d", intval));
#else
  new FXLabel(matrix,"-");
#endif


  label=new FXLabel(matrix,tr("Vertex Uniform Components:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
#if defined(GL_MAX_VERTEX_UNIFORM_COMPONENTS)
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &intval);
  new FXLabel(matrix, FXString::value("%d", intval));
#else
  new FXLabel(matrix,"-");
#endif


  label = new FXLabel(matrix,tr("Fragment Uniform Components:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
#if defined(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS)
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &intval);
  new FXLabel(matrix, FXString::value("%d", intval));
#else
  new FXLabel(matrix,"-");
#endif


  new FXTabItem(tabbook,tr(" Extensions "));
  vframe=new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);

  vframe=new FXVerticalFrame(vframe,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  extensionlist=new FXList(vframe,nullptr,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|HSCROLLING_OFF);

  // Get OpenGL extensions
  tmp=(char*)glGetString(GL_EXTENSIONS);
  if(tmp){
    text=strdup(tmp);
    token=strtok(text," ");
    while(token){
      extensionlist->appendItem(token);
      token=strtok(nullptr," ");
      }
    free(text);
    }

  extensionlist->setSortFunc(FXList::ascending);
  extensionlist->sortItems();

  // Get GLU extensions
#ifdef GLU_VERSION_1_1
  tmp=(char*)gluGetString(GLU_EXTENSIONS);
  if(tmp){
    text=strdup(tmp);
    token=strtok(text," ");
    while(token!=nullptr){
      extensionlist->appendItem(token);
      token=strtok(nullptr," ");
      }
    free(text);
    }
#endif

  new FXTabItem(tabbook,tr(" Driver "));
  matrix=new FXMatrix(tabbook, 2, MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK);

  label = new FXLabel(matrix,tr("Vendor:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%s",glGetString(GL_VENDOR)),nullptr,LABEL_NORMAL);

  label = new FXLabel(matrix,tr("Renderer:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%s",glGetString(GL_RENDERER)),nullptr,LABEL_NORMAL);

  label = new FXLabel(matrix,tr("GL Version:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%s",glGetString(GL_VERSION)),nullptr,LABEL_NORMAL);

  label = new FXLabel(matrix,tr("GLU Version:"),nullptr,LABEL_NORMAL|LAYOUT_RIGHT);
  label->setFont(thickfont);
  new FXLabel(matrix,FXString::value("%s",gluGetString(GLU_VERSION)),nullptr,LABEL_NORMAL);

  canvas->makeNonCurrent();
  }


// Save OpenGL info to a file
FXbool GLSettingsDialog::saveInformation(const FXString& filename){
  GLint	intval;
  GLint	intvals[2];

  FILE *fp=fopen(filename.text(),"w");
  if(fp){
    FXGLVisual *glvisual=(FXGLVisual*)canvas->getVisual();
    canvas->makeCurrent();
    fprintf(fp,"Driver Information\n");
    fprintf(fp,"------------------\n");
    fprintf(fp,"\n");
    fprintf(fp,"                       Vendor: %s\n",glGetString(GL_VENDOR));
    fprintf(fp,"                     Renderer: %s\n",glGetString(GL_RENDERER));
    fprintf(fp,"                   GL Version: %s\n",glGetString(GL_VERSION));
    fprintf(fp,"                  GLU Version: %s\n",gluGetString(GLU_VERSION));
    fprintf(fp,"\n");
    fprintf(fp,"Implementation Limits\n");
    fprintf(fp,"---------------------\n");
    fprintf(fp,"\n");
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, intvals);
    fprintf(fp,"                Viewport Size: %d x %d\n",intvals[0],intvals[1]);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &intval);
    fprintf(fp,"                 Texture Size: %d\n",intval);
#if defined(GL_MAX_TEXTURE_UNITS)
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &intval);
    fprintf(fp,"                Texture Units: %d\n",intval);
#endif
    glGetIntegerv(GL_MAX_LIGHTS, &intval);
    fprintf(fp,"                       Lights: %d\n",intval);
    glGetIntegerv(GL_MAX_CLIP_PLANES, &intval);
    fprintf(fp,"              Clipping Planes: %d\n",intval);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&intval);
    fprintf(fp,"              Modelview Stack: %d\n",intval);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,&intval);
    fprintf(fp,"             Projection Stack: %d\n",intval);
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH,&intval);
    fprintf(fp,"              Attribute Stack: %d\n",intval);
#if defined(GL_MAX_VERTEX_ATTRIBS)
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &intval);
    fprintf(fp,"            Vertex Attributes: %d\n",intval);
#else
    fprintf(fp,"            Vertex Attributes: -\n");
#endif
#if defined(GL_MAX_VERTEX_UNIFORM_COMPONENTS)
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &intval);
    fprintf(fp,"    Vertex Uniform Components: %d\n",intval);
#else
    fprintf(fp,"    Vertex Uniform Components: -\n");
#endif

#if defined(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS)
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &intval);
    fprintf(fp,"  Fragment Uniform Components: %d\n",intval);
#else
    fprintf(fp,"  Fragment Uniform Components: -\n");
#endif

    fprintf(fp,"\n");
    fprintf(fp,"Display Mode\n");
    fprintf(fp,"------------\n");
    fprintf(fp,"\n");
    fprintf(fp,"         Hardware Accelerated: %s\n",PRINT_YESNO(glvisual->isAccelerated()));
    fprintf(fp,"             Double Buffering: %s\n",PRINT_YESNO(glvisual->isDoubleBuffer()));
    fprintf(fp,"                  Stereo View: %s\n",PRINT_YESNO(glvisual->isStereo()));
    fprintf(fp,"          Buffer-swap by copy: %s\n",PRINT_YESNO(glvisual->isBufferSwapCopy()));
    fprintf(fp,"                  Color Depth: %d\n",glvisual->getActualRedSize()+glvisual->getActualGreenSize()+glvisual->getActualBlueSize()+glvisual->getActualAlphaSize());
    fprintf(fp,"            Depth Buffer Size: %d\n",glvisual->getActualDepthSize());
    fprintf(fp,"          Stencil Buffer Size: %d\n",glvisual->getActualStencilSize());
    fprintf(fp,"                         RGBA: %d-%d-%d-%d\n",glvisual->getActualRedSize(),glvisual->getActualGreenSize(),glvisual->getActualBlueSize(),glvisual->getActualAlphaSize());
    fprintf(fp,"                   Accum RGBA: %d-%d-%d-%d\n",glvisual->getActualAccumRedSize(),glvisual->getActualAccumGreenSize(),glvisual->getActualAccumBlueSize(),glvisual->getActualAccumAlphaSize());
    fprintf(fp,"                 Multi Sample: %d\n",glvisual->getActualMultiSamples());
    fprintf(fp,"\n");
    fprintf(fp,"Available Extensions\n");
    fprintf(fp,"--------------------\n");
    fprintf(fp,"\n");
    for(FXint i=0;i<extensionlist->getNumItems(); i++){
      fprintf(fp,"  %s\n",extensionlist->getItemText(i).text());
      }
    canvas->makeNonCurrent();
    fclose(fp);
    return true;
    }
  return false;
  }


// Save OpenGL info to file
long GLSettingsDialog::onCmdSave(FXObject*,FXSelector,void*){
  FXString filename=FXFileDialog::getSaveFilename(this,tr("Save Information"),FXSystem::getHomeDirectory()+PATHSEPSTRING+"opengl.txt",tr("Text Files (*.txt)\nAll Files(*.*)"));
  if(!filename.empty()){
    if(FXStat::exists(filename)){
      if(FXMessageBox::question(this,MBOX_YES_NO,tr("Overwrite File?"),tr("File %s already exists.\nWould you like to overwrite this file?"),filename.text())!=MBOX_CLICKED_YES)
        return 1;
      }
    if(!saveInformation(filename)){
      FXMessageBox::error(this,MBOX_OK,tr("Write Error"),tr("An error occured trying to write file."));
      }
    }
  return 1;
  }


// Destroy
GLSettingsDialog::~GLSettingsDialog(){
  delete thickfont;
  }


/*******************************************************************************/


// Event Handler Object
class GLTestWindow : public FXMainWindow {
  FXDECLARE(GLTestWindow)

private:

  FXGLCanvas    *glcanvas;      // GL Canvas to draw into
  FXRealSpinner *speedcontrol;  // Spinner for speed
  FXGLVisual    *glvisual;      // OpenGL visual
  FXdouble       rts;
  FXTime         lasttime;
  int            spinning;      // Is box spinning
  double         angle;         // Rotation angle of box
  FXDataTarget   dt_rts;

protected:
  GLTestWindow(){}

public:

  // We define additional ID's, starting from the last one used by the base class+1.
  // This way, we know the ID's are all unique for this particular target.
  enum{
    ID_CANVAS=FXMainWindow::ID_LAST,
    ID_SPIN,
    ID_SPINFAST,
    ID_STOP,
    ID_TIMEOUT,
    ID_CHORE,
    ID_OPENGL,
    ID_MULTISAMPLE_OFF,
    ID_MULTISAMPLE_2X,
    ID_MULTISAMPLE_4X,
    ID_SPEED
    };

  // Message handlers
  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
  long onMouseMove(FXObject*,FXSelector,void*);
  long onExpose(FXObject*,FXSelector,void*);
  long onConfigure(FXObject*,FXSelector,void*);
  long onCmdSpin(FXObject*,FXSelector,void*);
  long onUpdSpin(FXObject*,FXSelector,void*);
  long onCmdStop(FXObject*,FXSelector,void*);
  long onUpdStop(FXObject*,FXSelector,void*);
  long onTimeout(FXObject*,FXSelector,void*);
  long onChore(FXObject*,FXSelector,void*);
  long onCmdSpinFast(FXObject*,FXSelector,void*);
  long onUpdSpinFast(FXObject*,FXSelector,void*);
  long onCmdOpenGL(FXObject*,FXSelector,void*);
  long onCmdMultiSample(FXObject*,FXSelector,void*);
  long onUpdMultiSample(FXObject*,FXSelector,void*);
  long onUpdSpeed(FXObject*,FXSelector,void*);

public:

  // GLTestWindow constructor
  GLTestWindow(FXApp* a);

  // Initialize
  void create();

  // Draw scene
  void drawScene();

  // GLTestWindow destructor
  virtual ~GLTestWindow();
  };



// Message Map
FXDEFMAP(GLTestWindow) GLTestWindowMap[]={

  //________Message_Type_________ID_____________________Message_Handler_______
  FXMAPFUNC(SEL_PAINT,     GLTestWindow::ID_CANVAS,   GLTestWindow::onExpose),
  FXMAPFUNC(SEL_CONFIGURE, GLTestWindow::ID_CANVAS,   GLTestWindow::onConfigure),
  FXMAPFUNC(SEL_COMMAND,   GLTestWindow::ID_SPIN,     GLTestWindow::onCmdSpin),
  FXMAPFUNC(SEL_UPDATE,    GLTestWindow::ID_SPIN,     GLTestWindow::onUpdSpin),
  FXMAPFUNC(SEL_COMMAND,   GLTestWindow::ID_SPINFAST, GLTestWindow::onCmdSpinFast),
  FXMAPFUNC(SEL_UPDATE,    GLTestWindow::ID_SPINFAST, GLTestWindow::onUpdSpinFast),
  FXMAPFUNC(SEL_COMMAND,   GLTestWindow::ID_STOP,     GLTestWindow::onCmdStop),
  FXMAPFUNC(SEL_UPDATE,    GLTestWindow::ID_STOP,     GLTestWindow::onUpdStop),
  FXMAPFUNC(SEL_UPDATE,    GLTestWindow::ID_SPEED,     GLTestWindow::onUpdSpeed),
  FXMAPFUNC(SEL_TIMEOUT,   GLTestWindow::ID_TIMEOUT,  GLTestWindow::onTimeout),
  FXMAPFUNC(SEL_CHORE,     GLTestWindow::ID_CHORE,    GLTestWindow::onChore),
  FXMAPFUNC(SEL_COMMAND,   GLTestWindow::ID_OPENGL,   GLTestWindow::onCmdOpenGL),
  FXMAPFUNCS(SEL_COMMAND,  GLTestWindow::ID_MULTISAMPLE_OFF, GLTestWindow::ID_MULTISAMPLE_4X, GLTestWindow::onCmdMultiSample),
  FXMAPFUNCS(SEL_UPDATE,   GLTestWindow::ID_MULTISAMPLE_OFF, GLTestWindow::ID_MULTISAMPLE_4X,GLTestWindow::onUpdMultiSample),
  };



// Implementation
FXIMPLEMENT(GLTestWindow,FXMainWindow,GLTestWindowMap,ARRAYNUMBER(GLTestWindowMap))



// Construct a GLTestApp
GLTestWindow::GLTestWindow(FXApp* a):FXMainWindow(a,"OpenGL Test Application",nullptr,nullptr,DECOR_ALL,0,0,800,600){
  FXVerticalFrame *glcanvasFrame;
  FXVerticalFrame *buttonFrame;
  FXComposite *glpanel;
  FXGroupBox *groupbox;

  // RIGHT pane for the buttons
  buttonFrame=new FXVerticalFrame(this,LAYOUT_SIDE_RIGHT|LAYOUT_FILL_Y,0,0,0,0,2,2,3,3);

  // LEFT pane to contain the glcanvas
  glcanvasFrame=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,2,2,3,3);

  // Drawing glcanvas
  glpanel=new FXVerticalFrame(glcanvasFrame,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0, 0,0,0,0);

  // A Visual to drag OpenGL
  glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLE_BUFFER);

  // Drawing glcanvas
  glcanvas=new FXGLCanvas(glpanel,glvisual,this,ID_CANVAS,LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

  new FXButton(buttonFrame,tr("&OpenGL Info\tDisplay OpenGL Capabilities"),nullptr,this,ID_OPENGL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // Button to print
  new FXButton(buttonFrame,tr("Spin &Timer\tSpin using interval timers\nNote the app blocks until the interal has elapsed..."),nullptr,this,ID_SPIN,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);
  new FXButton(buttonFrame,tr("Spin &Chore\tSpin as fast as possible using chores\nNote even though the app is very responsive, it never blocks;\nthere is always something to do..."),nullptr,this,ID_SPINFAST,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // Button to print
  new FXButton(buttonFrame,tr("&Stop Spin\tStop this mad spinning, I'm getting dizzy"),nullptr,this,ID_STOP,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  groupbox = new FXGroupBox(buttonFrame,tr("Speed (rts)"),GROUPBOX_NORMAL|FRAME_GROOVE|LAYOUT_FILL_X);
  speedcontrol = new FXRealSpinner(groupbox,3,&dt_rts,FXDataTarget::ID_VALUE,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
  speedcontrol->setRange(0.1,3.0);
  speedcontrol->setIncrement(0.1);


  groupbox = new FXGroupBox(buttonFrame,tr("Multi Sampling"),GROUPBOX_NORMAL|FRAME_GROOVE|LAYOUT_FILL_X);
  new FXRadioButton(groupbox,"Off",this,ID_MULTISAMPLE_OFF);
  new FXRadioButton(groupbox,"2x",this,ID_MULTISAMPLE_2X);
  new FXRadioButton(groupbox,"4x",this,ID_MULTISAMPLE_4X);


  // Exit button
  new FXButton(buttonFrame,tr("&Exit\tExit the application"),nullptr,getApp(),FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_BOTTOM|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // Make a tooltip
  new FXToolTip(getApp());

  // Initialize private variables
  spinning=0;
  angle=0.0;
  rts=1.0;
  dt_rts.connect(rts);
  }


// Destructor
GLTestWindow::~GLTestWindow(){
  getApp()->removeTimeout(this,ID_TIMEOUT);
  getApp()->removeChore(this,ID_CHORE);
  delete glvisual;
  }


// Create and initialize
void GLTestWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
  }


// Widget has been resized
long GLTestWindow::onConfigure(FXObject*,FXSelector,void*){
  if(glcanvas->makeCurrent()){
    glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());
    glcanvas->makeNonCurrent();
    }
  return 1;
  }


// Widget needs repainting
long GLTestWindow::onExpose(FXObject*,FXSelector,void*){
  drawScene();
  return 1;
  }


// Rotate the boxes when a timer message is received
long GLTestWindow::onTimeout(FXObject*,FXSelector,void*){
  angle+=2.0;
  if(angle>360.0) angle-=360.0;
  lasttime=FXThread::time();
  drawScene();
  getApp()->addTimeout(this,ID_TIMEOUT,TIMER_INTERVAL);
  return 1;
  }


// Rotate the boxes when a chore message is received
long GLTestWindow::onChore(FXObject*,FXSelector,void*){
  FXTime c=FXThread::time();
  FXTime d=c-lasttime;
  angle+=(d/1000000000.0)*(360.0*rts);
  if(angle>360.0) angle-=360.0;
  lasttime=c;
  drawScene();
  getApp()->addChore(this,ID_CHORE);
  return 1;
  }


// Start the boxes spinning
long GLTestWindow::onCmdSpin(FXObject*,FXSelector,void*){
  spinning=1;
  getApp()->addTimeout(this,ID_TIMEOUT,TIMER_INTERVAL);
  return 1;
  }



// Enable or disable the spin button
long GLTestWindow::onUpdSpin(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable();
  return 1;
  }


// Start the boxes spinning
long GLTestWindow::onCmdSpinFast(FXObject*,FXSelector,void*){
  spinning=1;
  lasttime=FXThread::time();
  speedcontrol->enable();
  getApp()->addChore(this,ID_CHORE);
  return 1;
  }


// Enable or disable the spin button
long GLTestWindow::onUpdSpinFast(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->disable() : button->enable();
  return 1;
  }


// If boxes are spinning, stop them
long GLTestWindow::onCmdStop(FXObject*,FXSelector,void*){
  getApp()->removeTimeout(this,ID_TIMEOUT);
  getApp()->removeChore(this,ID_CHORE);
  speedcontrol->disable();
  spinning=0;
  return 1;
  }


// Enable or disable the stop button
long GLTestWindow::onUpdStop(FXObject* sender,FXSelector,void*){
  FXButton* button=(FXButton*)sender;
  spinning ? button->enable() : button->disable();
  return 1;
  }

// Enable or disable the stop button
long GLTestWindow::onUpdSpeed(FXObject* sender,FXSelector,void*){
  if (getApp()->hasTimeout(this,ID_TIMEOUT))
  sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
  else
  sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),nullptr);
  return 1;
  }


// Draws a simple box using the given corners
void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax) {
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,-1.);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(1.,0.,0.);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,1.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1.,0.,0.);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,1.,0.);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,-1.,0.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymin, zmin);
  glEnd();
  }


// Draw the GL scene
void GLTestWindow::drawScene(){
  const GLfloat lightPosition[]={15.,10.,5.,1.};
  const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
  const GLfloat lightDiffuse[]={.9f,.9f,.9f,1.};
  const GLfloat redMaterial[]={1.,0.,0.,1.};
  const GLfloat blueMaterial[]={0.,0.,1.,1.};

  GLdouble canvaswidth = glcanvas->getWidth();
  GLdouble canvasheight = glcanvas->getHeight();
  GLdouble aspect = canvasheight>0 ? canvaswidth/canvasheight : 1.0;

  // Make context current
  if(glcanvas->makeCurrent()){

    glViewport(0,0,glcanvas->getWidth(),glcanvas->getHeight());

    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_DITHER);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.,aspect,1.,100.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.,10.,15.,0.,0.,0.,0.,1.,0.);

    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);

    glPushMatrix();
    glRotated(angle, 0., 1., 0.);
    drawBox(-1, -1, -1, 1, 1, 1);

    glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redMaterial);

    glPushMatrix();
    glTranslated(0.,1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.,-1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPushMatrix();
    glRotated(90., 1., 0., 0.);
    glTranslated(0.,1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPushMatrix();
    glRotated(90., -1., 0., 0.);
    glTranslated(0.,1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPushMatrix();
    glRotated(90., 0., 0., 1.);
    glTranslated(0.,1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPushMatrix();
    glRotated(90., 0., 0., -1.);
    glTranslated(0.,1.75,0.);
    glRotated(angle, 0., 1., 0.);
    drawBox(-.5,-.5,-.5,.5,.5,.5);
    glPopMatrix();

    glPopMatrix();

    // Swap if it is double-buffered
    if(glvisual->isDoubleBuffer()){
      glcanvas->swapBuffers();
      }

    // Make context non-current
    glcanvas->makeNonCurrent();
    }
  }


// Pop a dialog showing OpenGL properties
long GLTestWindow::onCmdOpenGL(FXObject*,FXSelector,void*){
  GLSettingsDialog sd(this,glcanvas);
  sd.execute();
  return 1;
  }


// Switch multisampling on
long GLTestWindow::onCmdMultiSample(FXObject* sender,FXSelector sel,void*){
  FXint nsamples=0;
  switch(FXSELID(sel)){
    case ID_MULTISAMPLE_OFF: nsamples=0; break;
    case ID_MULTISAMPLE_2X : nsamples=2; break;
    case ID_MULTISAMPLE_4X : nsamples=4; break;
    }
  glcanvas->destroy();
  glvisual->destroy();
  glvisual->setMultiSamples(nsamples);
  glvisual->create();
  if(glvisual->getActualMultiSamples()!=nsamples){
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),nullptr);
    }
  glcanvas->create();
  return 1;
  }


// Update multisampling radio buttons
long GLTestWindow::onUpdMultiSample(FXObject* sender,FXSelector sel,void*){
  FXbool check=false;
  switch(FXSELID(sel)){
    case ID_MULTISAMPLE_OFF: if(glvisual->getActualMultiSamples()!=2 && glvisual->getActualMultiSamples()!=4) check=true; break;
    case ID_MULTISAMPLE_2X : if(glvisual->getActualMultiSamples()==2) check=true; break;
    case ID_MULTISAMPLE_4X : if(glvisual->getActualMultiSamples()==4) check=true; break;
    }
  if(check)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_CHECK),nullptr);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_UNCHECK),nullptr);
  return 1;
  }


// Here we begin
int main(int argc,char *argv[]){

  // Make application
  FXApp application("GLTest","FoxTest");

  // Open the display
  application.init(argc,argv);

  // Make window
  new GLTestWindow(&application);

  // Create the application's windows
  application.create();

  // Run the application
  return application.run();
  }


#else


// Here we begin
int main(int argc,char *argv[]){
  fxmessage("The FOX Library was compiled without OpenGL\n");
  return 0;
  }

#endif
