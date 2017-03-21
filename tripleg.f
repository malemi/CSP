      
      REAL FUNCTION TRIPLEG(X)  
      COMMON/PAWPAR/PAR(9)
      FUNC=PAR(1)*X +PAR(2)*EXP(-X)
      END
