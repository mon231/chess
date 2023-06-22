using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Threading;

namespace chessGraphics
{
    public partial class Form1 : Form
    {
        private Square srcSquare;
        private Square dstSquare;

        private pipe enginePipe;
        Piece[,] matBoard;

        bool isCurPlWhite = true;
        bool isGameOver = false;

        const int BOARD_SIZE = 9;

        public Form1()
        {
            InitializeComponent();
        }

        private void initForm()
        {
            enginePipe.connect();
            
            Invoke((MethodInvoker)delegate
            {

                lblWaiting.Visible = false;
                lblCurrentPlayer.Visible = true;
                label1.Visible = true;

                string s = enginePipe.getEngineMessage();

                if (s.Length != (BOARD_SIZE * BOARD_SIZE + 1))
                {
                    MessageBox.Show("The length of the board's string is not according the PROTOCOL");
                    this.Close();
                }
                else
                {
                    isCurPlWhite = (s.Last() == '0');
                    paintBoard(s);
                }
            });
        }

        Thread connectionThread;
        private void Form1_Load(object sender, EventArgs e)
        {
            enginePipe = new pipe();

            connectionThread = new Thread(initForm);
            connectionThread.Start();
            connectionThread.IsBackground = true;
        }

        Image getImageBySign(char sign)
        {
            switch (sign)
            {
                case 'q':
                    return Properties.Resources.q_black;
                case 'Q':
                    return Properties.Resources.q_white;
                case 'k':
                    return Properties.Resources.k_black;
                case 'K':
                    return Properties.Resources.k_white;
                case 'p':
                    return Properties.Resources.p_black;
                case 'P':
                    return Properties.Resources.p_white;
                case 'r':
                    return Properties.Resources.r_black;
                case 'R':
                    return Properties.Resources.r_white;
                case 'n':
                    return Properties.Resources.n_black;
                case 'N':
                    return Properties.Resources.n_white;
                case 'b':
                    return Properties.Resources.b_black;
                case 'B':
                    return Properties.Resources.b_white;
                case 'd':
                    return Properties.Resources.d_black;
                case 'D':
                    return Properties.Resources.d_white;
                case '#':
                    return null;
                default:
                    return Properties.Resources.x;
            }
        }

        private void paintBoard(string board)
        {
            int i, j, z = 0;

            matBoard = new Piece[BOARD_SIZE, BOARD_SIZE];
            btnBoard.FlatAppearance.MouseOverBackColor = Color.LightGray;

            Piece newBtn;
            Point pnt;

            int currentWidth = btnBoard.Location.X;
            int currentHeight = btnBoard.Location.Y;

            bool isColBlack = true;
            bool isRowBlack = true;

            this.SuspendLayout();

            lblCurrentPlayer.Text = isCurPlWhite ? "White" : "Black";

            for (i = 0; i < BOARD_SIZE; i++)
            {
                currentWidth = btnBoard.Location.X;
                isColBlack = isRowBlack;

                for (j = 0; j < BOARD_SIZE; j++)
                {
                    newBtn = new Piece(board[z]);
                    matBoard[i, j] = newBtn;

                    newBtn.FlatAppearance.MouseOverBackColor = btnBoard.FlatAppearance.MouseOverBackColor;
                    newBtn.BackColor = isColBlack ? Color.Gray : Color.White;
                    newBtn.FlatAppearance.BorderColor = btnBoard.FlatAppearance.BorderColor;
                    newBtn.FlatAppearance.BorderSize = btnBoard.FlatAppearance.BorderSize;
                    newBtn.FlatStyle = btnBoard.FlatStyle;

                    newBtn.Size = new Size(btnBoard.Width, btnBoard.Height);
                    newBtn.Tag = new Square(i, j);
                    pnt = new Point(currentWidth, currentHeight);
                    newBtn.Location = pnt;
                    newBtn.BackgroundImageLayout = ImageLayout.Stretch;

                    newBtn.BackgroundImage = getImageBySign(newBtn.GetPieceType());
                    newBtn.Click += lastlbl_Click;

                    Controls.Add(newBtn);

                    currentWidth += btnBoard.Width;
                    isColBlack = !isColBlack;
                    z++;
                }

                isRowBlack = !isRowBlack;
                currentHeight += btnBoard.Height;
            }

            Controls.Remove(btnBoard);
            this.ResumeLayout(false);
        }

        void lastlbl_Click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (srcSquare != null)
            {
                if (matBoard[srcSquare.Row, srcSquare.Col] == b)
                {
                    matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                    srcSquare = null;
                }
                else
                {
                    dstSquare = (Square)b.Tag;
                    matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.DarkGreen;

                    Thread t = new Thread(playMove);
                    t.Start();
                }
            }
            else
            {
                srcSquare = (Square)b.Tag;
                matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.DarkGreen;
            }
        }

        // messages should be according the protocol.
        // index is the message number in the protocol
        string[] messages =  {
            "Valid move",
            "Valid move - you made chess",
            "Invalid move - not your player",
            "Invalid move - destination is not free",
            "Invalid move - chess wil occure",
            "Invalid move - out of bounds",
            "Invalid move - illegeal movement with piece",
            "Invalid move - source and dest are equal",
            "Game over - check mate",
            "Unknown message"
            };

        string convertEngineToText(string m)
        {
            int res;
            bool b = int.TryParse(m, out res);

            if (!b || res < 0 || res >= messages.Length)
                return messages[messages.Length - 1];

            return messages[res];
        }

        void playMove()
        {
            if (isGameOver)
                return;

            try
            {
                Invoke((MethodInvoker)delegate
                {
                    lblEngineCalc.Visible = true;

                    lblMove.Text = string.Format("Move from {0} to {1}", srcSquare, dstSquare);
                    lblMove.Visible = true;

                    label2.Visible = false;
                    lblResult.Visible = false;

                    this.Refresh();

                    enginePipe.sendEngineMove(srcSquare.ToString() + dstSquare.ToString());
                    string m = enginePipe.getEngineMessage();

                    if (!enginePipe.isConnected())
                    {
                        MessageBox.Show("Connection to engine has lost. Bye bye.");
                        this.Close();
                        return;
                    }

                    string res = convertEngineToText(m);

                    if (res.ToLower().StartsWith("game over")) {
                        isGameOver = true;
                    } else if (res.ToLower().StartsWith("valid")) {
                        isCurPlWhite = !isCurPlWhite;
                        lblCurrentPlayer.Text = isCurPlWhite ? "White" : "Black";

                        /*If was a pown in last movement, and it achived to max place, replace it with a dragon*/
                        if ((dstSquare.Row == 0 || dstSquare.Row == BOARD_SIZE - 1) && matBoard[srcSquare.Row, srcSquare.Col].GetPieceType() == 'p') {
                            matBoard[dstSquare.Row, dstSquare.Col].BackgroundImage = getImageBySign('D');
                        } else if ((dstSquare.Row == 0 || dstSquare.Row == BOARD_SIZE - 1) && matBoard[srcSquare.Row, srcSquare.Col].GetPieceType() == 'P') {
                            matBoard[dstSquare.Row, dstSquare.Col].BackgroundImage = getImageBySign('d');
                        } else {
                            matBoard[dstSquare.Row, dstSquare.Col].BackgroundImage = matBoard[srcSquare.Row, srcSquare.Col].BackgroundImage;
                        }

                        matBoard[srcSquare.Row, srcSquare.Col].BackgroundImage = null;

                        matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                        matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                    }

                    lblEngineCalc.Visible = false;
                    lblResult.Text = string.Format("{0}", res);
                    lblResult.Visible = true;
                    label2.Visible = true;
                    this.Refresh();
                });
            } finally {
                Invoke((MethodInvoker)delegate {
                    if (srcSquare != null)
                        matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;

                    if (dstSquare != null)
                        matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.Blue;

                    dstSquare = null;
                    srcSquare = null;
                });
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            enginePipe.sendEngineMove("quit");
            enginePipe.close();
        }
    }
}