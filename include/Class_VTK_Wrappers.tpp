#include"Class_VTK_Wrappers.hpp"

//====================================================================================================
VtkUnstrVec::VtkUnstrVec()
             :VTK_UnstructuredGrid<VtkUnstrVec>(){};

//====================================================================================================
template< class T0, class T1>
VtkUnstrVec::VtkUnstrVec( string dir_, string name_, string codex_, uint8_t type_, vector<T0> &points_ext, vector<T1> &connectivity_ext )
             :VTK_UnstructuredGrid<VtkUnstrVec>( ){ 


    int ncells_, npoints_, nconn_;

    T0  dum0 ;
    T1  dum1 ;


    data.reserve(50) ;

    SetNames( dir_, name_ );
    SetCodex( codex_ ) ;

    type = type_ ;

    ncells_ = connectivity_ext.size() ;
    npoints_ = points_ext.size() ;

    nconn_ = ncells_ * NumberOfElements( type ) ;

    geometry[0].SetType( WhichType(dum0) ) ;
    geometry[1].SetType("UInt64") ;
    geometry[2].SetType("UInt8") ;
    geometry[3].SetType( WhichType(dum1) ) ;

    SetDimensions( connectivity_ext.size(), points_ext.size(), nconn_ ) ;

    adata.resize(4) ;

    adata[0].DPtr = &points_ext ;
    adata[1].DPtr = static_cast<vector<int32_t>*>(NULL) ;
    adata[2].DPtr = static_cast<vector<int32_t>*>(NULL) ;
    adata[3].DPtr = &connectivity_ext ;

};

//====================================================================================================
VtkUnstrVec::VtkUnstrVec( string dir_, string name_, string codex_, uint8_t type_ )
             :VTK_UnstructuredGrid<VtkUnstrVec>( ){ 

    data.reserve(50) ;

    SetNames( dir_, name_ );
    SetCodex( codex_ ) ;

    type = type_ ;

};


//====================================================================================================
VtkUnstrVec::~VtkUnstrVec(){

};

//====================================================================================================
template<class T>
void VtkUnstrVec::AddData( vector<T> &data_, string name_, string loc_ ){

    int n = adata.size() ;
    T               dum_ ;

    adata.push_back( ufield() ) ;

    adata[n].name = name_ ;
    adata[n].DPtr = &data_ ;

    VTK_UnstructuredGrid<VtkUnstrVec>::AddData( name_, 1, WhichType(dum_), loc_ ) ;

    return;
};

//====================================================================================================
template<class T>
void VtkUnstrVec::AddData( vector< array<T,3> > &data_, string name_, string loc_ ){

    int n = adata.size() ;
    T               dum_ ;

    adata.push_back( ufield() ) ;

    adata[n].name = name_ ;
    adata[n].DPtr = &data_ ;

    VTK_UnstructuredGrid<VtkUnstrVec>::AddData( name_, 3, WhichType(dum_), loc_ ) ;

    return;
};

//====================================================================================================
template<class T>
void VtkUnstrVec::AddData( vector< vector<T> > &data_, string name_, string loc_ ){

    int n = adata.size() ;
    T               dum_ ;

    adata.push_back( ufield() ) ;

    adata[n].name = name_ ;
    adata[n].DPtr = &data_ ;

    VTK_UnstructuredGrid<VtkUnstrVec>::AddData( name_, 3, WhichType(dum_), loc_ ) ;


    return;
};

//====================================================================================================
template<class T>
void VtkUnstrVec::LinkData( vector<T> &data_, string name_ ){

    int n = adata.size() ;

    adata.push_back( ufield() ) ;

    adata[n].name = name_ ;
    adata[n].DPtr = &data_ ;


    return;
};

// =================================================================================== //
bool VtkUnstrVec::GetFieldByName( const string &name_, VtkUnstrVec::ufield *&the_field ){

    vector<VtkUnstrVec::ufield>::iterator    it_ ;

    for( it_=adata.begin(); it_!=adata.end(); ++it_){
        if( (*it_).name == name_ ){
            the_field = &(*it_) ;
            return true ;
        };
    };

    return false ;
};

//====================================================================================================
void VtkUnstrVec::Flush( fstream &str, string codex, string name ) {


    if( codex == "ascii" && name == "types"){
        for( uint64_t n=0; n<nr_cells-1; n++) {
            flush_ascii( str, type  ) ;
            str << endl ;
        };
        flush_ascii( str, type  ) ;
    }

    else if( codex == "binary" && name == "types"){
        for( uint64_t n=0; n<nr_cells; n++) flush_binary( str, type  ) ;
    }

    else if( codex == "ascii" && name == "offsets"){
        uint64_t off_(0) ;
        for(uint64_t  n=0; n<nr_cells-1; n++) {
            off_ += NumberOfElements( type ) ; 
            flush_ascii( str, off_  ) ;
            str << endl ;
        };
        off_ += NumberOfElements( type ) ;
        flush_ascii( str, off_  ) ;
    }

    else if( codex == "binary" && name == "offsets"){
        uint64_t off_(0), nT( NumberOfElements( type ) ) ;
        for( uint64_t n=0; n<nr_cells; n++) {
            off_ += nT ; 
            flush_binary( str, off_  ) ;
        };
    }

    else{

        ufield  *f_ ;

        if( GetFieldByName( name, f_)){
            stream_visitor  visitor;
            visitor.SetStream( str ) ;
            visitor.SetCodex( codex ) ;
            visitor.SetName( name ) ;
            visitor.SetTask( "write" ) ;

            boost::apply_visitor(visitor, f_->DPtr ); 
        };

    };

    return ;

};

//====================================================================================================
void VtkUnstrVec::Absorb( fstream &str, string codex, string name ) {

    ufield  *f_ ;

    if( GetFieldByName( name, f_)){

        VTK::Field_C*   FPtr ;
        if( VTK::GetFieldByName(name, FPtr) ){

            stream_visitor  visitor;
            visitor.SetStream( str ) ;
            visitor.SetCodex( codex ) ;
            visitor.SetName( name ) ;
            visitor.SetTask( "read" ) ;

            visitor.SetSize( FPtr->GetElements() ) ;
            visitor.SetComponents( FPtr->GetComponents() ) ;

            if( name == "connectivity") visitor.SetComponents( NumberOfElements(type) ) ;

            if( ! (boost::get<vector<int32_t>*>(&f_->DPtr) && boost::get<vector<int32_t>*>(f_->DPtr) == NULL ) ){
                boost::apply_visitor(visitor, f_->DPtr ); 
            };
        };
    }

    return ;

};
