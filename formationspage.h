/**
 * Has all the images and buttons to acces different managers
 * and teams in the formations and styles section
 * @author John Laird and Ishan Sharma
 * @date 4/24/2025
 */
#ifndef FORMATIONSPAGE_H
#define FORMATIONSPAGE_H

#include <QWidget>

namespace Ui {
class FormationsPage;
}

class FormationsPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent
     */
    explicit FormationsPage(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~FormationsPage();

signals:
    /**
    * Ferguson's button gets clicked
    */
    void fergusonClicked();
    /**
    * Klopp's button gets clicked
    */
    void kloppClicked();
    /**
    * Guardiola's button gets clicked
    */
    void guardiolaClicked();
    /**
    * Ancelotti's button gets clicked
    */
    void ancelottiClicked();
    /**
    * Mourinho's button gets clicked
    */
    void mourinhoClicked();
    /**
    * Simeone's button gets clicked
    */
    void simeoneClicked();
    /**
    * Cruyff's button gets clicked
    */
    void cruyffClicked();
    /**
    * Zidane's button gets clicked
    */
    void zidaneClicked();
    /**
    * Alonso's button gets clicked
    */
    void alonsoClicked();
    /**
    * Conte's button gets clicked
    */
    void conteClicked();

    /**
    * First Barcelona team's button gets clicked
    */
    void firstBarcaClicked();
    /**
    * Real Madrid's button gets clicked
    */
    void madridClicked();
    /**
    * Inter Milan's button gets clicked
    */
    void interClicked();
    /**
    * Ajax's button gets clicked
    */
    void ajaxClicked();
    /**
    * Liverpool's button gets clicked
    */
    void liverpoolClicked();
    /**
    * Leicester City's button gets clicked
    */
    void leicesterClicked();
    /**
    * Second Barcelona team's button gets clicked
    */
    void secondBarcaClicked();

public slots:
    /**
    * Slot triggered when Ferguson button is clicked
    */
    void on_fergusonButton_clicked();
    /**
    * Slot triggered when Klopp button is clicked
    */
    void on_kloppButton_clicked();
    /**
    * Slot triggered when Guardiola button is clicked
    */
    void on_guardiolaButton_clicked();
    /**
    * Slot triggered when Ancelotti button is clicked
    */
    void on_ancelottiButton_clicked();
    /**
    * Slot triggered when Mourinho button is clicked
    */
    void on_mourinhoButton_clicked();
    /**
    * Slot triggered when Simeone button is clicked
    */
    void on_simeoneButton_clicked();
    /**
    * Slot triggered when Cruyff button is clicked
    */
    void on_cruyffButton_clicked();
    /**
    * Slot triggered when Zidane button is clicked
    */
    void on_zidaneButton_clicked();
    /**
    * Slot triggered when Alonso button is clicked
    */
    void on_alonsoButton_clicked();
    /**
    * Slot triggered when Conte button is clicked
    */
    void on_conteButton_clicked();

    /**
    * Slot triggered when first Barcelona team button is clicked
    */
    void on_firstBarcaButton_clicked();
    /**
    * Slot triggered when Real Madrid button is clicked
    */
    void on_madridButton_clicked();
    /**
    * Slot triggered when Inter Milan button is clicked
    */
    void on_interButton_clicked();
    /**
    * Slot triggered when Ajax button is clicked
    */
    void on_ajaxButton_clicked();
    /**
    * Slot triggered when Liverpool button is clicked
    */
    void on_liverpoolButton_clicked();
    /**
    * Slot triggered when Leicester City button is clicked
    */
    void on_leicesterButton_clicked();
    /**
    * Slot triggered when second Barcelona team button is clicked
    */
    void on_secondBarcaButton_clicked();

private:
    /**
     * UI
     */
    Ui::FormationsPage *ui;
};

#endif // FORMATIONSPAGE_H
