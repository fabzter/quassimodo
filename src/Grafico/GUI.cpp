#include "GUI.hpp"
#include <boost/filesystem.hpp>

using namespace irr;

Grafico::GUI::GUI(scene::ISceneManager *smgr, gui::IGUIEnvironment *env,
                  Skin *Skin) {
  this->smgr = smgr;
  this->env = env;
  this->skin = Skin;
  this->botonAgente.resize(2);
  this->botonPartida.resize(BP_COUNT);
  this->AvsA = 0;
  this->T_Pausa = NULL;
  botonesPartida = false;
  this->setSkin();
}

Grafico::GUI::GUI(const GUI &orig) {}

Grafico::GUI::~GUI() {
  this->dropBotonesPartida();
  this->dropAvsA();
  this->dropTextPausa();
  this->op = NULL;
}
void Grafico::GUI::setSkin() {
  // gui::IGUISkin* skin = this->env->createSkin(gui::EGST_WINDOWS_METALLIC);
  /*gui::IGUISkin* skin =this->env->getSkin();

   //skin->setFont(this->skin->getGUIWindow(),gui::EGDF_WINDOW);

   skin->setFont(this->skin->getGUIBoton() ,gui::EGDF_BUTTON);
   skin->setFont(this->skin->getDefault(),gui::EGDF_DEFAULT);



skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,255,255,255));
*/
  gui::IGUISkin *sskin = this->skin->getSkinGui();
  sskin->setFont(this->skin->getDefault(), gui::EGDF_DEFAULT);
  sskin->setFont(this->skin->getMenuToolTip(), gui::EGDF_TOOLTIP);
  // ponemos mas opaco los colores de la GUI
  for (u32 i = 0; i < gui::EGDC_COUNT; ++i) {
    video::SColor col = sskin->getColor((gui::EGUI_DEFAULT_COLOR)i);
    col.setAlpha(150);
    sskin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
  }

  sskin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255, 255, 255, 255));

  this->env->setSkin(sskin);
}
void Grafico::GUI::MsgBox(const char * /*msg*/, GUI_BOTONES_OK /*idMsg*/) {
  // Stubbed: addMessageBox/EMBF_OK/IGUIWindow removed from IrrlichtMt
}
void Grafico::GUI::AgntVSAgnt() { this->dibujaSelector(true); }
void Grafico::GUI::AgntVSMkn() { this->dibujaSelector(false); }
std::string Grafico::GUI::getPath() {
  wchar_t *m = (wchar_t *)op->getFileName();
  boost::filesystem::wpath w_path(m);

  return std::string(w_path.string().begin(), w_path.string().end());
}

void Grafico::GUI::OpenFileDialog() {
  op = this->env->addFileOpenDialog(L"Selecciona el Agente", true, 0, 5);
}

void Grafico::GUI::dropAvsA() {
  if (this->AvsA != 0) {
    if (this->botonAgente[0]) {
      this->botonAgente[0]->remove();
      this->botonAgente[0] = 0;
    }
    if (this->botonAgente[1]) {
      this->botonAgente[1]->remove();
      this->botonAgente[1] = 0;
    }
    // D2: Don't remove root GUI element
    if (this->AvsA != this->env->getRootGUIElement())
      this->AvsA->remove();
    this->AvsA = 0;
  }
}

void Grafico::GUI::dibujaSelector(bool ambos) {
  core::dimension2d<unsigned int> S_S =
      core::dimension2d<unsigned int>(600, 310);
  int d_a =
      (this->smgr->getVideoDriver()->getScreenSize().Width - S_S.Width) / 2;
  int d_al =
      (this->smgr->getVideoDriver()->getScreenSize().Height - S_S.Height) / 2;
  core::rect<s32> recW =
      core::rect<s32>(d_a, d_al, S_S.Width + d_a, S_S.Height + d_al);

  // D2: IrrlichtMt removed addWindow. Use root GUI element as parent.
  gui::IGUIElement *root = this->env->getRootGUIElement();
  this->AvsA = root;

  this->env->addStaticText(
      L"Vs",
      core::rect<s32>((recW.getWidth() / 2) - 15, (recW.getHeight() / 2) - 30,
                      (recW.getWidth() / 2) + 15, (recW.getHeight() / 2) + 40),
      false, false, root);

  this->env->addStaticText(L"Selecciona los Agentes",
                           core::rect<s32>(35, 35, 300, 60), false, false,
                           root);

  this->botonAgente[0] = this->env->addButton(
      core::rect<s32>((recW.getWidth() / 2) - 185, (recW.getHeight() / 2) - 50,
                      (recW.getWidth() / 2) - 35, (recW.getHeight() / 2)),
      root, BA_AGENTE_1, L"Agente?");
  this->botonAgente[1] = this->env->addButton(
      core::rect<s32>((recW.getWidth() / 2) + 40, (recW.getHeight() / 2) - 50,
                      (recW.getWidth() / 2) + 190, (recW.getHeight() / 2)),
      root, BA_AGENTE_2, L"Agente?");
  if (!ambos) {
    this->botonAgente[1]->setText(L"Agente MinMax");
    this->botonAgente[1]->setEnabled(ambos);
    this->botonAgente[1]->setPressed(!ambos);
  }
  this->env->addButton(
      core::rect<s32>(recW.getWidth() - 360, recW.getHeight() - 60,
                      recW.getWidth() - 190, recW.getHeight() - 20),
      root, BO_CANCELA, L"Cancelar");
  this->env->addButton(
      core::rect<s32>(recW.getWidth() - 160, recW.getHeight() - 60,
                      recW.getWidth() - 20, recW.getHeight() - 20),
      root, BO_INICIA, L"Inicia");
}

void Grafico::GUI::setTextAgnt(int num, const char *text) {
  if (num >= 0 && num < 2 && this->botonAgente[num]) {
    std::wstring ws(text, text + strlen(text));
    this->botonAgente[num]->setText(ws.c_str());
  }
}
void Grafico::GUI::setBotonesPartida() {

  core::dimension2d<unsigned int> Scren_Size =
      this->smgr->getVideoDriver()->getScreenSize();
  int dis_ancho = 20, dis_alto = 20;
  float size = 50, dan = dis_ancho;
  for (int i = 0; i < BP_COUNT; i++) {

    this->botonPartida.at(i) = this->env->addButton(
        core::rect<s32>(dan, dis_alto, dan + size, dis_alto + size), 0,
        i + 1000, L"");
    dan += size + 10;
    this->botonPartida.at(i)->setDrawBorder(true);
    this->botonPartida.at(i)->setImage(this->skin->getBotonPartida(i));
    this->botonPartida.at(i)->setPressedImage(
        this->skin->getBotonPartidaPres(i));
    this->botonPartida.at(i)->setUseAlphaChannel(true);
  }
  this->botonPartida.at(BP_COUNT - 1)
      ->setToolTipText(GUI_BOTONES_TOLTIP_PARTIDA[0]);
  botonesPartida = true;
}
void Grafico::GUI::dropBotonesPartida() {
  if (botonesPartida) {
    for (int i = 0; i < BP_COUNT; i++) {
      this->botonPartida.at(i)->remove();
      this->botonPartida.at(i) = 0;
    }
    botonesPartida = false;
  }
}

void Grafico::GUI::Pausar(bool pausa) {
  int ultpos = this->botonPartida.size();
  if (pausa) {
    this->botonPartida.at(ultpos - 2)
        ->setImage(this->skin->getBotonPartida(ultpos));
    this->botonPartida.at(ultpos - 2)
        ->setPressedImage(this->skin->getBotonPartidaPres(ultpos));
    this->setTextPausa();
  } else {
    this->botonPartida.at(ultpos - 2)
        ->setImage(this->skin->getBotonPartida(ultpos - 2));
    this->botonPartida.at(ultpos - 2)
        ->setPressedImage(this->skin->getBotonPartidaPres(ultpos - 2));
    this->dropTextPausa();
  }
  this->botonPartida.at(ultpos - 2)->setUseAlphaChannel(true);
  this->botonPartida.at(ultpos - 2)->setDrawBorder(true);
}

void Grafico::GUI::setTextPausa() {
  core::dimension2d<unsigned int> S_S =
      core::dimension2d<unsigned int>(100, 50);
  int d_a =
          (this->smgr->getVideoDriver()->getScreenSize().Width - S_S.Width) / 2,
      d_al =
          (this->smgr->getVideoDriver()->getScreenSize().Height - S_S.Height) /
          2;
  core::rect<s32> rec =
      core::rect<s32>(d_a, d_al, S_S.Width + d_a, S_S.Height + d_al);

  core::dimension2d<unsigned int> SS =
      this->smgr->getVideoDriver()->getScreenSize();
  this->T_Pausa = this->env->addStaticText(L"PAUSA", rec);
  this->T_Pausa->setOverrideFont(this->skin->getMenuBoton());
  this->T_Pausa->setOverrideColor(video::SColor(
      255, 130, 0,
      0)); // this->smgr->addTextSceneNode(this->skin->getMenuBoton(),,video::SColor(255,255,255,255)
           // );
}

void Grafico::GUI::dropTextPausa() {

  if (this->T_Pausa != NULL || this->T_Pausa != 0) {
    this->T_Pausa->remove();
    this->T_Pausa = NULL;
  }
}

void Grafico::GUI::setCreditos() {
  // core::position2d<s32> pos;
  core::dimension2d<u32> S_S = this->skin->getImagenCreditos()->getSize();

  int d_a =
          (this->smgr->getVideoDriver()->getScreenSize().Width - S_S.Width) / 2,
      d_al =
          (this->smgr->getVideoDriver()->getScreenSize().Height - S_S.Height) /
          2;
  this->creditos =
      this->env->addImage(this->skin->getImagenCreditos(),
                          core::position2d<s32>(d_a, d_al), true, 0);
  this->env->addButton(core::rect<s32>(815, 495, 895, 525), this->creditos,
                       BO_CREDITOS, L"OK");
}

void Grafico::GUI::dropCreditos() { this->creditos->remove(); }
