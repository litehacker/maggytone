#include "maggytone.h"
//COMPILE g++ `pkg-config --cflags opencv` -o maggytone-tree maggytone-tree.cpp `pkg-config --libs opencv` -pthread
//RUN ./fractal-tree


Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255));     // Background colors

namespace MaggyTone
{

    void Image::initialize()
    {
        start.x = w/2;                              // Starting at x coordinate
        start.y = h/2;                              // Starting at y coordinate
        maggytone = "MaggyTone Project";
    }
    Image::Image()
    {
        initialize();
    }



    Buffer::Buffer()
    {
        flag=false;
        data[MAX_NOTE_NUMBER]={1};
        press[MAX_NOTE_NUMBER]={0};
        mut_lock=0;
    }
    void Buffer::unmodify()
    {
        flag=false;
        data[MAX_NOTE_NUMBER]={0};           // delete all old input data
        for (int i = 0; i<MAX_NOTE_NUMBER; i++)
        data[i]=0;
    }
    bool Buffer::modified()
    {
        if (flag)
            return 1;
        return 0;
    }
    void Buffer::modify()
    {
        flag=true;
    }
    void Buffer::clear_buffer()
    {
        data[MAX_NOTE_NUMBER]={0};
    }
    bool Buffer::pressed(int location)
    {
        if (press[location])
        {
            return 1;
        }
        return 0;
    }
    void Buffer::unpress(int location)
    {
        press[location]=0;
    }
    void Buffer::lock(){
        mut_lock=1;
        buf_mutex.lock();
    }
    void Buffer::unlock(){
        buf_mutex.unlock();
        mut_lock=0;
    }
    bool Buffer::locked(){
        if (mut_lock==1)
            return 1;
        return 0;
    }




    bool busy()
    {
        //check it timer is >0.01 s
        //if yes return true
        return 0; //demo
    }


    /*
    Input value is the needed row of the matrix which represents
    exact time of notes played.
    */
    void add_value_to_matrix(
    	int matrix[][MAX_NOTE_NUMBER],				//the row to be updated
    	vector <int> starts,	//start positions
    	int st_n,
    	vector <int> ends,	//end positions
    	int ed_n,
    	int counter,
    	int force)
    {

    	//use matrix value as the address of 61 set array
    	//Mutex starts
    	while(st_n>0)
    	{
    		st_n--;
    		matrix[counter][starts[st_n]]=force;
    	}

    	while(ed_n>0)
    	{
    		ed_n--;
    		matrix[counter][ends[ed_n]]=-1;
    	}
    	//Mutex ends
    }


    void print_matrix(
    	int matrix[LENGTH][MAX_NOTE_NUMBER])
    {
    	int i,j;

        for(i=0; i<LENGTH; i++)
        {
            for(j=0; j<MAX_NOTE_NUMBER; j++)
            {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }


    //maggytone read input from piano main function
    void read_input()
    {
        char byte;

        int note,force,matrix[1000][MAX_NOTE_NUMBER]={0},to_int=0, counter=0,fd = open("/dev/midi1", O_RDWR);
        short state=-1;

        vector <int> starts;	//start notes
        vector <int> ends;		//end notes

        while(counter<LENGTH)
        {
    	    ssize_t size = read(fd, &byte, 1);
    	    to_int=byte;

    //	    printf("%d\n", to_int);
    	    if(to_int==-112)
    		{
    	    	printf("START\n");
    	    	state =1;
    	    }
    	    else if(to_int==-128)
    	    {
    	    	printf("END\n");
    	    	state =0;
    	    }
    	    else
    	    {
                if(state==2)
    	    	{
    	    		state=3;
    	    		if(!busy())
       		  		{
       		  			force=to_int;
       		  			add_value_to_matrix(matrix,starts,starts.size(),ends,ends.size(),counter,force);
       		  			counter++;
       		  			starts.clear();
       		  			ends.clear();
       		  		}
    	    	}
    	    	else
    	    	if(state==1)
    	    	{
    	    		starts.push_back(to_int-36);
    	    		state=2;
    	    	}
    	    	else
    	    	if(state==0)
                {
    	    		ends.push_back(to_int-36);
    	    		state=3;
    	    		if(!busy())
    	    		{
       		  			add_value_to_matrix(matrix,starts,starts.size(),ends,ends.size(),counter,0);
    	    			counter++;
    	    			starts.clear();
       		  			ends.clear();
    				}
    	    	}
    	    	else if(state==-1)
                {
    	    		printf("Not connected!\n");
    	    	}
    	    	printf("%d                 %d\n", to_int,counter);
    	    }
        }
        print_matrix(matrix);
    }



    // Continue drawing if the buffer has any of the input
    bool continue_drawing()
    {

        if (!buffer.modified())
            return 1;
        else
            return 0;
    }

    // Drawing filled circles in specific location
    void draw_line(double size, double angle)
    {
        int color[3]={0};
        double x_coord = image.start.x, y_coord = image.start.y;
        //buffer.unlock();

        angle=(angle*DEGREES/MAX_NOTE_NUMBER);

        while(continue_drawing()){
            circle(bckg, image.start, 1, Scalar( color[0],color[1],color[2] ), FILLED,LINE_8 );

            moveWindow( image.maggytone, 0, 0 );
            imshow( image.maggytone, bckg );
            waitKey(100);

            //when to update size
            if(size>1)
                size-=0.1;

            //update coordinates
            x_coord += sin(angle*pi/180);
            y_coord += -cos(angle*pi/180);

            image.start.x = x_coord;
            image.start.y = y_coord;
        }
        std::cout << "---------------------------: buffer.modified()=" <<buffer.modified()<< '\n';

    }

    void print_buf()
    {
        for (int i = 0; i<MAX_NOTE_NUMBER; i++)
        cout << buffer.data[i]<<" ";
        cout<<endl;
    }

    void *fractal(void *)
    {
        int angle, button, size;
        while(1==1)
        {
            if (buffer.modified())        // If buffer is modified
            {
                //buffer.lock();
                for (int k = 0; k < MAX_NOTE_NUMBER; k++)
                {
                    if (buffer.data[k]>0)
                    {
                        size=buffer.data[k];
                        angle=k;
                        break;
                    }
                }
                buffer.unmodify();                  // Disable flag and reset buffer
                print_buf();
                draw_line( size, angle);
            }
        }
        //fractal(NULL);
        pthread_exit(0);
    }



    void *update_buffer(void*)
    {
        long i=0;
        int button;
        while (1==1)
        {
            // some action to tigger to update the buffer
            while(buffer.modified()){}
            cin>>button;
            // actual update starts here. lock and unlock for safety
            #ifdef BUF_LOCK
            //lock_guard <mutex> lockGuard(buffer.buf_mutex); //Do this for each buffer you want to access

            buffer.lock();
            #endif

            buffer.data[button]=button;    // mark pressed button
            buffer.modify();               // enable modified flag

            #ifdef BUF_LOCK

            buffer.unlock();

            #endif
            if (buffer.modified())
                cout<<"buffer updated successfully"<< i <<":"<<buffer.modified()<<endl;
            i++;
        }
        pthread_exit(0);
    }
}
