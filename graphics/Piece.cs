using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace chessGraphics
{
    class Piece : Button
    {
        private char type;

        public Piece(char type)
        {
            this.type = type;
        }

        public char GetPieceType()
        {
            return this.type;
        }
    }
}
