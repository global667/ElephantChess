// TODO 3 [ ]
// Pruefe den Zug auf Gueltigkeit, wenn das Spiel im TURNIERMODUS* laeuft, 
// indem du alle moeglichen Zuge
// probierst und die gueltigen Zuege speicherst. Wenn der ausgefuehrte
// Zug in dieser Menge ist, so ist er gueltig.
// *TURNIERMODUS an: Zugkontrolle aus, Chinesische Zeichen, Uhr
//  TURNIERMODUS aus: Zugkontrolle, frei waehlbare Steine, keine Uhr/Uhr 
// TODO 2b [ ]
// Implementiere den noetigen Overhead, um mit der Engine zu kommunzieren
// -------------------------------------------------------------------------------------
// BUGS 
// [ ] Steine gehen manchmal verloren, wenn sie schlagen.
//     Das Problem dabei ist, dass die geschlagenen Steine nicht geloescht werden
//     und weiter auf dem Brett existieren, weil der schlagende Stein nicht
//     "weiss", dass er einen Stein geschlagen hat. Die Loesung muss es also sein 
//     den geschlagen Stein vom Brett zu entfernen.
//     Sinnvoll waere eine Funktion isEmty(x,y), die vor den Ziehen prueft, ob das
//     Zielfeld leer ist und ggf. die noetigen Schritte einleitet und die darauf stehende
//     Figur loescht.
//     Anmerkung: Das Problem besteht nur bei den schwarzen Steinen. Diese werden in den 
//     entsprechenden Methoden zuerst abgefragt.
//
// [x] Startbildschirm zeigt fehlerhaftes Brett.

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.lang.*;
import java.awt.Graphics2D;
import java.awt.Graphics;

import ucichess.UCIChess;

/*
Koordinaten sind falsch, folgen aber der Java-Konvention!! Sie beginnen oben Links.
Konvention, laut UCI, ist aber der Beginn unten Links.
        Schwarz
    0 1 2 3 4 5 6 7 8 |
    1                 |
    2                 |
    3                 |
    4                 |
    ==================|
    5                 |
    6                 |
    7                 |
    8                 |
    9                 |
    ------------------+
        Rot
ES SOLLTE ABER, LAUT UCI, SO SEIN; WOBEI ZAHLEN 
ODER BUCHSTABEN JEWEILS OPTIONAL SIND:
        
        Schwarz
    ------------------+
    9                 | 
    8                 |
    7                 |
    6                 |
    5                 |
    ================= |
    4                 |
    3                 |
    2                 |
    1                 |
    a0 b c d e f g h i|
        Rot
*/
public class XiangQi extends JFrame implements MouseListener {
    JPanel boardPanel = new BoardPanel();
    JToolBar toolPanel = new ToolBar();
    Board board = new Board();
    doWithStones doWithStone;
    UCIChess uci;
    String moveList;
    int xx = -10, yy = -10;
    int X = -10, Y = -10;
    int press = 0;
    int selectedX = -10, selectedY = -10;

    // Wg. der Raender
    double boardSizeWidth = boardPanel.getWidth() - 50;
    double boardSizeHeight = boardPanel.getHeight() - 40;
    // Durch die Anzahl der Schnittpunkt
    double squareWidth = 90;//boardSizeWidth / 9.0;
    double squareHeight = 90;//boardSizeHeight / 10.0;
    double boardCursorX;
    double boardCursorY;

    String[] pngStoneNames = new String[]
            {
                    "canonBlack", "rookBlack", "longKnightBlack",
                    "knightBlack", "guardBlack", "kingBlack", "pawnBlack",
                    "canonRed", "rookRed", "longKnightRed", "knightRed", "guardRed",
                    "kingRed", "pawnRed"
            };
    Stone canonBlack, canonBlack2, rookBlack, rookBlack2, longKnightBlack,
            longKnightBlack2, knightBlack, knightBlack2, guardBlack, guardBlack2,
            kingBlack, pawnBlack, pawnBlack2, pawnBlack3, pawnBlack4, pawnBlack5;
    Stone canonRed, canonRed2, rookRed, rookRed2, longKnightRed,
            longKnightRed2, knightRed, knightRed2, guardRed, guardRed2,
            kingRed, pawnRed, pawnRed2, pawnRed3, pawnRed4, pawnRed5;
    Stone[] stones;

    static class Stone {
        int x, y;

        public Stone(int x, int y) {
            this.x = x;
            this.y = y;
        }

        public void setX(int x) {
            this.x = x;
        }

        public void setY(int y) {
            this.y = y;
        }

    }

    public class Board {

        Board() {
            initBoard();
        }

        public void initBoard () {
                rookRed = new Stone(0, 9);
                knightRed = new Stone(1, 9);
                longKnightRed = new Stone(2, 9);
                guardRed = new Stone(3, 9);
                guardRed2 = new Stone(5, 9);
                kingRed = new Stone(4, 9);
                longKnightRed2 = new Stone(6, 9);
                knightRed2 = new Stone(7, 9);
                rookRed2 = new Stone(8, 9);
                canonRed = new Stone(1, 7);
                canonRed2 = new Stone(7, 7);
                pawnRed = new Stone(0, 6);
                pawnRed2 = new Stone(2, 6);
                pawnRed3 = new Stone(4, 6);
                pawnRed4 = new Stone(6, 6);
                pawnRed5 = new Stone(8, 6);
                rookBlack = new Stone(0, 0);
                knightBlack = new Stone(1, 0);
                longKnightBlack = new Stone(2, 0);
                guardBlack = new Stone(3, 0);
                guardBlack2 = new Stone(5, 0);
                kingBlack = new Stone(4, 0);
                longKnightBlack2 = new Stone(6, 0);
                knightBlack2 = new Stone(7, 0);
                rookBlack2 = new Stone(8, 0);
                canonBlack = new Stone(1, 2);
                canonBlack2 = new Stone(7, 2);
                pawnBlack = new Stone(0, 3);
                pawnBlack2 = new Stone(2, 3);
                pawnBlack3 = new Stone(4, 3);
                pawnBlack4 = new Stone(6, 3);
                pawnBlack5 = new Stone(8, 3);
                stones = new Stone[]
                        {
                                canonBlack, canonBlack2, rookBlack, rookBlack2, longKnightBlack,
                                longKnightBlack2, knightBlack, knightBlack2, guardBlack, guardBlack2,
                                kingBlack, pawnBlack, pawnBlack2, pawnBlack3, pawnBlack4, pawnBlack5,
                                canonRed, canonRed2, rookRed, rookRed2, longKnightRed,
                                longKnightRed2, knightRed, knightRed2, guardRed, guardRed2,
                                kingRed, pawnRed, pawnRed2, pawnRed3, pawnRed4, pawnRed5
                        };
        }
    }

    public class BoardPanel extends JPanel {
        public BoardPanel() {
            super();
        }

        Image getPNG(String PNGname) {
            for (String name : pngStoneNames) {
                if (name.equals(PNGname)) {
                    String stoneImageName = name + ".png";
                    ImageIcon stoneIcon = new ImageIcon(this.getClass().getResource(stoneImageName));
                    return stoneIcon.getImage();
                }
            }
            return null;
        }

        public void paint(Graphics g) {
            paintBoard(g);
            try {
                g.drawImage(getPNG("rookBlack"), rookBlack.x * 90 + 20, rookBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("longKnightBlack"), longKnightBlack.x * 90 + 20, longKnightBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("knightBlack"), knightBlack.x * 90 + 20, knightBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("guardBlack"), guardBlack.x * 90 + 20, guardBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("guardBlack"), guardBlack2.x * 90 + 20, guardBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("kingBlack"), kingBlack.x * 90 + 20, kingBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("longKnightBlack"), longKnightBlack2.x * 90 + 20, longKnightBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("knightBlack"), knightBlack2.x * 90 + 20, knightBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("rookBlack"), rookBlack2.x * 90 + 20, rookBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("canonBlack"), canonBlack.x * 90 + 20, canonBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("canonBlack"), canonBlack2.x * 90 + 20, canonBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnBlack"), pawnBlack.x * 90 + 20, pawnBlack.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnBlack"), pawnBlack2.x * 90 + 20, pawnBlack2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnBlack"), pawnBlack3.x * 90 + 20, pawnBlack3.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnBlack"), pawnBlack4.x * 90 + 20, pawnBlack4.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnBlack"), pawnBlack5.x * 90 + 20, pawnBlack5.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("rookRed"), rookRed.x * 90 + 20, rookRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("longKnightRed"), longKnightRed.x * 90 + 20, longKnightRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("knightRed"), knightRed.x * 90 + 20, knightRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("guardRed"), guardRed.x * 90 + 20, guardRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("guardRed"), guardRed2.x * 90 + 20, guardRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("kingRed"), kingRed.x * 90 + 20, kingRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("longKnightRed"), longKnightRed2.x * 90 + 20, longKnightRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("knightRed"), knightRed2.x * 90 + 20, knightRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("rookRed"), rookRed2.x * 90 + 20, rookRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("canonRed"), canonRed.x * 90 + 20, canonRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("canonRed"), canonRed2.x * 90 + 20, canonRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnRed"), pawnRed.x * 90 + 20, pawnRed.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnRed"), pawnRed2.x * 90 + 20, pawnRed2.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnRed"), pawnRed3.x * 90 + 20, pawnRed3.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnRed"), pawnRed4.x * 90 + 20, pawnRed4.y * 90 + 20, 60, 60, null);
                g.drawImage(getPNG("pawnRed"), pawnRed5.x * 90 + 20, pawnRed5.y * 90 + 20, 60, 60, null);
            } catch (Exception e) {
                e.printStackTrace();
            }
            for (Stone stone : stones) {
                if (stone.x == selectedX && stone.y == selectedY) {
                    Graphics2D g2 = (Graphics2D) g;
                    g2.setStroke(new BasicStroke(5));
                    g2.setColor(Color.GREEN);
                    g2.drawOval(selectedX * 90 + 10, selectedY * 90, 80, 80);
                    return;
                }
            }
        }

        // TODO Markierungen fuer Soldaten und Kanonen zeichen
        // TODO Unter Rand
        // TODO Liniendicke 
        public void paintBoard(Graphics g) {
            Color background = new Color(252, 175, 62);
            Color sides = new Color(206, 92, 0);
            Graphics2D g2 = (Graphics2D) g;
            // Hintergrund (Brett)
            g2.setColor(background);
            g2.fillRect(0, 0, 820, 950);
            // Gewoehnliche Schnittpunkte
            g2.setStroke(new BasicStroke(2));
            g2.setColor(Color.black);
            for (int i = 0; i < 9; i++) {
                g2.drawLine(i * 90 + 50, 40, i * 90 + 50, 810 + 40);
            }
            for (int i = 0; i < 10; i++) {
                g2.drawLine(0 + 50, i * 90 + 40, 720 + 50, i * 90 + 40);
            }
            // Palaeste
            g2.setColor(Color.black);
            g2.drawLine(270+50, 0+40, 450+50, 180+40);
            g2.drawLine(270+50, 180+40, 450+50, 0+40);
            g2.drawLine(270+50, 630+40, 450+50, 810+40);
            g2.drawLine(270+50, 810+40, 450+50, 630+40);
            // Fluss
            g2.setColor(background);
            g2.fillRect(50, 360 + 40, 810 - 90, 90);
            // Seitenraender
            g2.setColor(sides);
            // Linker Rand
            g2.fillRect(0, 0, 49, 851);
            // Unterer Rand
            g2.fillRect(0, 851, 950, 54);
            // Rechter Rand
            g2.fillRect(771, 0, 49, 851);
            // Oberer Rand
            g2.fillRect(49, 0, 850, 40);

            g2.setColor(Color.blue);
            g2.fillRect((int)(squareWidth*X), (int)(squareHeight*Y-squareHeight), (int)squareWidth, (int)squareHeight);

        }

    }

    public class ToolBar extends JToolBar {
        public ToolBar() {
            JButton b = new JButton("Neu");
            b.addActionListener(e -> {
                moveList = "";
                xx = X = selectedX = -10;
                yy = Y = selectedY = -10;
                press = 0;
                board.initBoard();
                boardPanel.repaint();
            });
            JButton c = new JButton("Abwaehlen");
            c.addActionListener(e -> {
                xx = X = selectedX = -10;
                yy = Y = selectedY = -10;
                press = 0;
                boardPanel.repaint();
            });
            add(b);
            addSeparator();
            add(c);
            repaint();
        }
    }

    @FunctionalInterface // optional
    public interface doWithStones {
        void doIt(int x, int y, int x2, int y2, Stone s);
    }

    public XiangQi() {
        super("XiangQi - Chinesisches Schach");
        //initUCI();
        addMouseListener(this);
        setGUI();
    }

    public void initUCI() {
        uci = new UCIChess("./chameleon");
        if (!uci.get_UciOk(true)) {
            System.out.println("Keine UCI-Engine vorhanden; No UCI-Engine available.");
            return;
        }
        moveList = "";
    }
    
    // NUR FUER TESTZWECKE (Junit)
    public int mousePressedX(int e) {
        // Wg. der Raender
        boardSizeWidth = boardPanel.getWidth();
        boardSizeHeight = boardPanel.getHeight();
        // Durch die Anzahl der Schnittpunkt
        squareWidth = boardSizeWidth / 9.0;
        squareHeight = boardSizeHeight / 10.0;
        boardCursorX = e - (squareWidth / 2);
        boardCursorY = e;
        X = (int) ((double) Math.round((boardCursorX / squareWidth) )); //(int) ((double) e.getX() / (double) 90);
        Y = (int) ((double) Math.round(boardCursorY / squareHeight));//(double) 90);        
        return X;
    }  
    
    // NUR FUER TESTZWECKE (Junit)
    public int mousePressedY(int e) {
        // Wg. der Raender
        boardSizeWidth = boardPanel.getWidth();
        boardSizeHeight = boardPanel.getHeight();
        // Durch die Anzahl der Schnittpunkt
        squareWidth = boardSizeWidth / 9.0;
        squareHeight = boardSizeHeight / 10.0;
        boardCursorX = e - (squareWidth / 2);
        boardCursorY = e - (squareHeight / 2);
        X = (int) ((double) Math.round(boardCursorX / squareWidth )); //(int) ((double) e.getX() / (double) 90);
        Y = (int) ((double) Math.round(boardCursorY / squareHeight));//(double) 90);        
        return Y;
    }    
 

    // Die Funktion, die unter anderem die Mauszeiger-Daten an das UCI-Protokoll
    // uebergibt, wo sie dann von der Engine verarbeitet
    // werden
    @Override
    public void mousePressed(MouseEvent e) {
        // Wg. der Raender
        boardSizeWidth = boardPanel.getWidth();
        boardSizeHeight = boardPanel.getHeight();
        // Durch die Anzahl der Schnittpunkt
        squareWidth = boardSizeWidth / 9.0;
        squareHeight = boardSizeHeight / 10.0;
        boardCursorX = e.getX() - (squareWidth / 2);
        boardCursorY = e.getY() - (squareHeight / 2);

        System.out.println("squareWidth = " + squareWidth);
        System.out.println("squareHeight = " + squareHeight);

        if (press == 0) {
            X = (int) ((double) Math.round(boardCursorX / squareWidth )); //(int) ((double) e.getX() / (double) 90);
            Y = (int) ((double) Math.round(boardCursorY / squareHeight));//(double) 90);
            System.out.println("e.getX(): " + e.getX());
            System.out.println("e.getY(): " + e.getY());
            System.out.println("X = " + X);
            System.out.println("Y = " + Y);
            selectedX = X;
            selectedY = Y;
            press = 1;
            repaint();
            selectedX = -10;
            selectedY = -10;
            press = 0;
        } else if (press == 1) {
           /* selectedX = -10;
            selectedY = -10;
            press = 0;
            xx =  (int) ((double) boardCursorX / squareWidth);
            yy = (int) ((double) boardCursorY / squareHeight);
            if ((xx == X) && (yy == Y)) return;
            stoneOps(X, Y, xx, yy, 1);
            stoneOps(X, Y, xx, yy, 2);
            repaint();
            */
        }
    }

    private void sendToUCI(int from_x, int from_y, int to_x, int to_y) {
        // Konvertiert in UCI-moves Liste
        String notation = convertToUCINotation(from_x, from_y, to_x, to_y);
        moveList = moveList + notation + " ";
        // Sendet die Zueg als Liste an das UCI-Protokoll.
        uci.move_FromSTART(moveList, true);
        uci.go_Think();
        getFromUCI();
    }

    private void getFromUCI() {
        String notation = uci.get_BestMove(true);
        System.out.println(notation);
        moveList = moveList + notation + " ";
        // Konvertiert den erhaltenen Bestmove in fuer das Programm 
        // verstaendliche Koordinaten (siehe Darstellung weiter oben)
        String numbers = convertToOwnNotation(notation);
        System.out.println(numbers);
        String[] number = numbers.split("");
        int x1 = Integer.parseInt(number[0]);
        int y1 = Integer.parseInt(number[1]);
        int x2 = Integer.parseInt(number[2]);
        int y2 = Integer.parseInt(number[3]);
        stoneOps(x1, y1, x2, y2, 1);
        stoneOps(x1, y1, x2, y2, 3);
    }

    // Wandelt die Buchstaben in Zahlen um. Dreht das Brett entsprechend der internen
    // Notation
    private String convertToOwnNotation(String notation) {
        //Anmerkung: abs(y-9) konvertiert programminternes Format nach UCI
        String[] number = notation.split("");
        int x1 = (int) notation.charAt(0);
        // Hack: Konvertiert Buchstaben, char, in Zahl, int
        x1 = x1 - 'a';
        int y1 = Integer.parseInt(number[1]);
        y1 = Math.abs(y1 - 9);
        int x2 = (int) notation.charAt(2);
        x2 -= 'a';
        int y2 = Integer.parseInt(number[3]);
        y2 = Math.abs(y2 - 9);
        notation = Integer.toString(x1) + Integer.toString(y1) + Integer.toString(x2) + Integer.toString(y2);
        return notation;
    }

    private String convertToUCINotation(int from_x, int from_y, int to_x, int to_y) {
        //abs(y-9) konvertiert programminternes Format nach UCI
        System.out.println("from_x: " + from_x + "from_y: " + from_y);
        System.out.println("to_x: " + to_x + "to_y: " + to_y);
        String x1 = Character.toString((char) from_x + 'a');
        System.out.println("x1: " + x1);
        String y1 = Integer.toString(Math.abs(from_y - 9));
        String x2 = Character.toString((char) to_x + 'a');
        String y2 = Integer.toString(Math.abs(to_y - 9));
        return x1 + y1 + x2 + y2;
    }

    // Operationen mit Steinen
// TODO gibt false zurück, wenn keine passende Figur gefunden wurde.    
    private void stoneOps(int X, int Y, int xx, int yy, int modus) {
        switch (modus) {
            case 1:
                // Tagged eine Figur zum loeschen
                System.out.println("modus 1");
                doWithStone = (x, y, x2, y2, s) -> {
                    if (s.x == x2 && s.y == y2) {
                        s.setX(-1000);
                        s.setY(y2);
                    }
                };
                break;
            case 2:
                // Ruft die Engine auf und setzt eine Figur
                System.out.println("modus 2");
                doWithStone = (x, y, x2, y2, s) -> {
                    if (s.x == x && s.y == y) {
                        sendToUCI(x, y, x2, y2);
                        s.setX(x2);
                        s.setY(y2);
                    }
                };
                break;
            case 3:
                // Bewegt die entsprechende Figur
                System.out.println("modus 3");
                doWithStone = (x, y, x2, y2, s) -> {
                    if (s.x == x && s.y == y) {
                        s.setX(x2);
                        s.setY(y2);
                    }
                };
                break;
            default:
                System.out.println("Ein Fehler ist aufgetreten.");
                break;
        }
        // Eine for-Schleife, die alle Steine durchlaeuft und mit einer Lambda-Funktion ausführt.
        for (Stone stone : stones) {
            doWithStone.doIt(X, Y, xx, yy, stone);
        }
        repaint();
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void paint(Graphics g) {
        boardPanel.repaint();
        toolPanel.repaint();
    }

    public void setGUI() {
        setResizable(false);
        setSize(820, 950);
        //board.setSize(880, 900);
        Container contentPane = getContentPane();
        contentPane.add(toolPanel, BorderLayout.NORTH);
        contentPane.add(boardPanel, BorderLayout.CENTER);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //this.pack();
        this.setVisible(true);
    }

    public static void main(String[] args) {
        //Schedule a job for the event dispatch thread:
        //creating and showing this application's GUI.
        SwingUtilities.invokeLater(() -> {
            //Turn off metal's use of bold fonts
            UIManager.put("swing.boldMetal", Boolean.FALSE);
            new XiangQi();
        });
    }  
}  
