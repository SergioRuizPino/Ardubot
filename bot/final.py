import serial
import tkinter
import time

ventana=tkinter.Tk()
global ventanacontrol
#arlec = serial.Serial('COM10', 9600)  #recibe de arduino
aresc=serial.Serial('COM11', 9600) #envia a arduino en mi pc
aresc.bytesize=serial.EIGHTBITS
aresc.timeout=1
time.sleep(2)
global celdas
global vel
global distancia
global tim
global obst
global mov
global c
global x
X=360
global y
Y=360

def crearventana():
	
	ventana.title("Robot 1.0V")
	ventana.config(bg="cyan")
	etiqueta = tkinter.Label(ventana,text="CELDAS RECORRIDAS ",font=('ARIAL', 16),bg="cyan")
	etiqueta2 = tkinter.Label(ventana,text="VELOCIDAD CM/S  ",font=('ARIAL', 16),bg="cyan")
	etiqueta3 = tkinter.Label(ventana,text="DISTANCIA EN CM ",font=('ARIAL', 16),bg="cyan")
	etiqueta4 = tkinter.Label(ventana,text="TIEMPO TRANSCURRIDO ",font=('ARIAL', 16),bg="cyan")
	etiqueta5 = tkinter.Label(ventana,text="OBSTACULO CELDA ",font=('ARIAL', 16),bg="cyan")
	boton = tkinter.Button(ventana, text="Actualizar Datos Robot", command=actualizar, width=20, height=0, bg="orange")
	boton2 = tkinter.Button(ventana, text="Controlar Robot desde PC", command=moverrobot, width=20, height=0, bg="orange")
	boton3 = tkinter.Button(ventana, text="Comenzar", command=auto, width=20, height=0, bg="orange")
	global log 
	global log2
	global log3
	global log4
	global log5
	global c
	log= tkinter.Text (ventana, width=20, height=0, takefocus=0)
	log2= tkinter.Text (ventana, width=20, height=0, takefocus=0)
	log3= tkinter.Text (ventana, width=20, height=0, takefocus=0)
	log4= tkinter.Text (ventana, width=20, height=0, takefocus=0)
	log5= tkinter.Text (ventana, width=20, height=0, takefocus=0)
	etiqueta.grid(row=1,column=1)
	etiqueta5.grid(row=2,column=1)
	etiqueta2.grid(row=3,column=1)
	etiqueta3.grid(row=4,column=1)
	etiqueta4.grid(row=5,column=1)
	boton.grid(row=6,column=1)
	boton2.grid(row=6,column=3)
	boton3.grid(row=6,column=2)
	log.grid(row=1,column=2)
	log2.grid(row=2,column=2)
	log3.grid(row=3,column=2)
	log4.grid(row=4,column=2)
	log5.grid(row=5,column=2)
	ventana.geometry("800x500")
	global tim
	tim=time.time()
	global c
	c = tkinter.Canvas(ventana, bg="yellow", height=300, width=300)
	c.grid(row=9,column=2)
	c.create_rectangle(0,0, 300, 300, width=5)
	line1 = c.create_line(60,0 , 60, 300)
	line2 = c.create_line(120,0 , 120, 300)
	line3 = c.create_line(180,0 , 180, 300)
	line4 = c.create_line(240,0 , 240, 300)
	line5 = c.create_line(0,60 , 300, 60)
	line5 = c.create_line(0,120 , 300, 120)
	line6 = c.create_line(0,180 , 300, 180)
	line7 = c.create_line(0,240 , 300, 240)
	ventana.maxsize(800,500)
	ventana.mainloop()
	

def actualizar():
	time.sleep(1)
	actualizardatos()
	#actualizarcanvas()
	

def moverrobot():
	global ventanacontrol
	ventanacontrol=tkinter.Toplevel(ventana)
	ventanacontrol.title("Controlbot")
	ventanacontrol.transient(ventana)
	ventanacontrol.config(bg="cyan")
	ventanacontrol.geometry("400x300+500+300")
	ventanacontrol.maxsize(400,300)
	b = tkinter.Button(ventanacontrol, text="MOVER", command=adelante, width=20, height=0, bg="orange")
	b2 = tkinter.Button(ventanacontrol, text="GIRARDRCH", command=derecha, width=20, height=0, bg="orange")
	b3 = tkinter.Button(ventanacontrol, text="GIRARIZQ", command=izquierda, width=20, height=0, bg="orange")
	b4 = tkinter.Button(ventanacontrol, text="SALIR", command=salirr, width=20, height=0, bg="orange")
	b5 = tkinter.Button(ventanacontrol, text="ATRAS", command=atras, width=20, height=0, bg="orange")
	b.grid(row=1,column=1)
	b2.grid(row=2,column=2)
	b3.grid(row=1,column=2)
	b4.grid(row=2,column=1)
	b5.grid(row=3,column=1)
	ventanacontrol.protocol("WM_DELETE_WINDOW",ventana.destroy)
	time.sleep(0.3)
	aresc.write(str.encode('1'))

def actualizarcanvas():
	global x
	global y
	global mov
	global c
	if(mov=="d"):
		x=x+60
		c.create_rectangle(x,y, x+60, y+60, width=5,fill="green");
	if(mov=="i"):
		x=x-60
		c.create_rectangle(x,y, x+60, y-60, width=5,fill="green");
	if(mov=="b"):
		y=y+60
		c.create_rectangle(x,y, x+60, y+60, width=5,fill="green");
	if(mov=="s"):
		y=y-60
		c.create_rectangle(x,y, x+60, y-60, width=5,fill="green");
	c.after(50)
	c.update()	



def adelante():
	aresc.write(str.encode('1'))
	time.sleep(1)



def derecha():
	aresc.write(str.encode('2'))
	time.sleep(1)

def izquierda():
	aresc.write(str.encode('3'))
	time.sleep(1)

def salirr():
	aresc.write(str.encode('4'))
	global ventanacontrol
	ventanacontrol.destroy()
	print("saliendo")

def atras():
	aresc.write(str.encode('5'))
	time.sleep(1)

def auto():
	aresc.write(str.encode('0'))
	time.sleep(1)
	tim= time.time()
	#meta=arlec.read(1)
#	while(meta==false):
	#actualizardatos()
	#	actualizarcanvas()
	
def actualizardatos():
		
	global tim
	global mov
	aresc.setDTR()
	seg=time.time()-tim

	celdas=aresc.readline()
	log.delete('0.0','20.0')
	log.insert('0.0',celdas)


	vel=aresc.readline()
	log3.delete('0.0','20.0')
	log3.insert('0.0',vel);

	distancia=aresc.readline()
	log4.delete('0.0','4.0')
	log4.insert('0.0',distancia);


	log5.delete('0.0','20.0')
	log5.insert('0.0',seg);

	mov=aresc.readline() 

	obst = aresc.readline()
	log2.delete('0.0','20.0')
	log2.insert('0.0',obst);
	
	
	aresc.flushInput()
	actualizarcanvas()
	
	


crearventana()



	