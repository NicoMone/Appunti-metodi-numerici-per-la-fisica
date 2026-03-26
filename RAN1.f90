!!! RAN1.F90
! questa è solo una routine di esempio
REAL FUNCTION ran1(IDUM)
    ! Generatore di numeri pseudo-casuali uniformi in (0,1)
    ! Se IDUM < 0 oppure se è la prima chiamata, la routine
    ! inizializza lo stato interno del generatore.
    implicit none
    
    integer, intent(inout) :: idum
    integer :: j
    integer, save :: iff = 0
    integer, save :: ix1, ix2, ix3
    real, save :: r(97)
    ! Queste variabili devono conservare il loro valore tra chiamate successive alla funzione.
    ! Parametri (m, a, c) dei tre generatori congruenziali:
    integer, parameter :: m1 = 259200, ia1 = 7141, ic1 = 54773
    integer, parameter :: m2 = 134456, ia2 = 8121, ic2 = 28411
    integer, parameter :: m3 = 243000, ia3 = 4561, ic3 = 51349
    real,    parameter :: rm1 = 3.8580247e-6
    real,    parameter :: rm2 = 7.4373773e-6

    IF (idum < 0 .OR. iff = 0) THEN
        iff=1
        ix1=mod(ic1-idum,m1)
        ix1=mod(ia1*ix1+ic1,m1)
        ix2=mod(ix1,m2)
        ix1=mod(ia1*ix1+ic1,m1)
        ix3=mod(ix1,m3)
        DO j=1, 97
            ix1=mod(ia1*ix1+ic1,m1)
            ix2=mod(ia2*ix2+ic2,m2)
            r(j)=(REAL(ix1)+REAL(ix2)*rm2)*rm1
        CONTINUE
        idum=1
    ENDIF

    ix1=mod(ia1*ix1+ic1,m1)  ! Aggiornamento dei tre generatori congruenziali
    ix2=mod(ia2*ix2+ic2,m2)
    ix3=mod(ia3*ix3+ic3,m3)

    j=1+(97*ix3)/m3 ! Selezione dell'elemento della tabella di shuffle

    IF(j > 97 .OR. j < 1) then
        stop "Errore in ran1: indice fuori intervallo"
    ENDIF
    ! Controllo di sicurezza: j dovrebbe sempre stare tra 1 e 97.

    ran1=r(j)
    ! Il numero pseudo-casuale restituito è l elemento j-esimo della tabella di shuffle.

    ! L elemento appena estratto viene sostituito con un nuovo valore costruito dai due generatori ix1 e ix2.
    ! Questo è il cuore del meccanismo di shuffle.
    r(j)=(REAL(ix1)+REAL(ix2)*rm2)*rm1
    RETURN

END